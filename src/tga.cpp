#include "tga.h" 
#include <cstdint>
#include <cstring>
#include <vector>

template<typename Dumper_t, typename Pixel_t>
bool DumpCompressedTGA(const uint8_t* inbuf, size_t len, GLsizei width, GLsizei height, GLsizei bpp, Dumper_t& DumpPixel, Pixel_t& PixelIndex)
{
	uint8_t buf[1 + 4 * 128];
	uint8_t* pb = buf + 1;

	size_t pixelcount = width * height;
	size_t currentpixel = 0;
	do
	{
		uint8_t& chunkheader = buf[0];

		memcpy((char*)buf, inbuf, 1 + bpp);
		inbuf += 1 + bpp;

		if (chunkheader < 128)
		{
			if (chunkheader > 0)
			{
				memcpy((char*)pb + bpp, inbuf, bpp * chunkheader);
				inbuf += bpp * chunkheader;

			}
			++chunkheader;

			if (currentpixel + chunkheader > pixelcount)
			{
				return false;
			}
			for (size_t counter = 0; counter < chunkheader; ++counter)
			{
				DumpPixel(PixelIndex(currentpixel), pb + bpp * counter);

				currentpixel++;
			}
		}
		else
		{
			chunkheader -= 127;

			if (currentpixel + chunkheader> pixelcount)
			{
				return false;
			}
			for (short counter = 0; counter < chunkheader; ++counter)
			{
				DumpPixel(PixelIndex(currentpixel), pb);

				++currentpixel;
			}
		}
	} while (currentpixel < pixelcount);
	
	return true;
}

static bool LoadUncompressedTGA(const uint8_t* inbuf, size_t len, GLsizei& width, GLsizei& height, GLenum& format, uint8_t*& image)
{
	uint8_t header[6];
	memcpy((char*)header, inbuf, sizeof(header));
	inbuf += sizeof(header);
	len -= sizeof(header);

	width  = header[1] * 256 + header[0];
	height = header[3] * 256 + header[2];
	int bpp = header[4];

	bool flip = (0 == (header[5] & 0x20));

	if((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp !=32)))
	{
		return false;
	}

	if(24 == bpp)
	{
		format = GL_RGB;
	}
	else
	{
		format = GL_RGBA;
	}

	bpp /= 8;
	size_t size = width * height * bpp;
	image = new uint8_t[size];
	if (!flip)
	{
		memcpy((char*)image, inbuf, size);
		inbuf += size;
		len -= size;
	}
	else
	{
		size_t lineSize = width * bpp;
		for(size_t i = 0;i < (size_t)height;++i)
		{
			memcpy((char*)(image + (height - i - 1) * lineSize), inbuf, lineSize);
			inbuf += lineSize;
			len -= lineSize;
		}
	}

	// Byte Swapping Optimized By Steve Thomas
	for(size_t cswap = 0; cswap < size; cswap += bpp)
	{
		image[cswap] ^= image[cswap + 2] ^= image[cswap] ^= image[cswap + 2];
	}
	return true;
}

static bool LoadCompressedTGA(const uint8_t* inbuf, size_t len, GLsizei& width, GLsizei& height, GLenum& format, uint8_t*& image)
{ 
	uint8_t header[6];

	memcpy((char*)header, inbuf, sizeof(header));
	inbuf += sizeof(header);
	len -= sizeof(header);

	width  = header[1] * 256 + header[0];
	height = header[3] * 256 + header[2];
	int bpp = header[4];

	bool flip = (0 == (header[5] & 0x20));

	if ((width <= 0) || (height <= 0) || ((bpp != 24) && (bpp != 32)))
	{
		return false;
	}

	if(24 == bpp)
	{
		format = GL_RGB;	
	}
	else
	{
		format = GL_RGBA;	
	}

	bpp /= 8;

	size_t size = width * height * bpp;
	image = new uint8_t[size];
	uint8_t* pixels = image;

	auto Dump3BPPPixel = [&pixels](size_t index, const uint8_t* buf)
	{
		pixels[index + 0] = buf[2];
		pixels[index + 1] = buf[1];
		pixels[index + 2] = buf[0];
	};

	auto Dump4BPPPixel = [&pixels](size_t index, const uint8_t* buf)
	{
		pixels[index + 0] = buf[2];
		pixels[index + 1] = buf[1];
		pixels[index + 2] = buf[0];
		pixels[index + 3] = buf[3];
	};

	auto NoFlipPixelIndex = [width, height, bpp](size_t pixel)->size_t
	{
		return pixel * bpp;
	};

	auto FlipPixelIndex = [width, height, bpp](size_t pixel)->size_t
	{
		return ((height - 1 - pixel / width) * width + (pixel % width)) * bpp;
	};

	if (bpp == 3)
	{
		if (!flip)
		{
			if (!DumpCompressedTGA(inbuf, len, width, height, bpp, Dump3BPPPixel, NoFlipPixelIndex))
			{
				return false;
			}
		}
		else
		{
			if (!DumpCompressedTGA(inbuf, len, width, height, bpp, Dump3BPPPixel, FlipPixelIndex))
			{
				return false;
			}
		}
	}
	else
	{
		if (!flip)
		{
			if (!DumpCompressedTGA(inbuf, len, width, height, bpp, Dump4BPPPixel, NoFlipPixelIndex))
			{
				return false;
			}
		}
		else
		{
			if (!DumpCompressedTGA(inbuf, len, width, height, bpp, Dump4BPPPixel, FlipPixelIndex))
			{
				return false;
			}
		}
	}

	return true;
}

bool LoadTGA(const char* name, GLsizei& width, GLsizei& height, GLenum& format, uint8_t*& image)
{
	std::vector<uint8_t> buffer;
	{
		std::ifstream inf(name, std::ios::binary | std::ios::in);
		if(!inf)
		{
			return 0;
		}
		inf.seekg(0, inf.end);
		buffer.resize(inf.tellg());
		inf.seekg(0, inf.beg);
		inf.read((char*)&buffer[0], buffer.size());
	}

	uint8_t uTGAcompare[12] = {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	uint8_t cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

	uint8_t header[12];

	memcpy((char*)header, &buffer[0], sizeof(header));
	uint8_t* buf = &buffer[sizeof(header)];
	size_t   len = buffer.size() - sizeof(header);
	
	bool result = false;
	if(0 == memcmp(uTGAcompare, header, sizeof(header)))
	{
		result = LoadUncompressedTGA(buf, len, width, height, format, image);
	}
	else if(0 == memcmp(cTGAcompare, header, sizeof(header)))
	{
		result = LoadCompressedTGA(buf, len, width, height, format, image);
	}
	return result;
}
