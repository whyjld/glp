#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <FreeImagePlus.h>
#include "texture.h"
#include "tga.h"

Texture::Texture(const char* file)
 : GLObj(CreateObject())
 , m_Type(GL_TEXTURE_2D)
 , m_Width(0)
 , m_Height(0)
{
	if(!Load(file))
	{
	    DeleteObject(m_Obj);
	    throw("Can't load texture file.");
	}
}

Texture::Texture(GLenum type)
 : GLObj(CreateObject())
 , m_Type(type)
 , m_Width(0)
 , m_Height(0)
{
    glBindTexture(type, m_Obj);
}

Texture::Texture(const Texture& v)
 : GLObj(v)
 , m_Type(v.m_Type)
 , m_Width(v.m_Width)
 , m_Height(v.m_Height)
{
}
	
Texture::Texture(Texture&& v)
 : GLObj(v)
 , m_Type(v.m_Type)
 , m_Width(v.m_Width)
 , m_Height(v.m_Height)
{
}

Texture::~Texture()
{
}

void Texture::Bind()
{
	glBindTexture(m_Type, m_Obj);
}

GLuint Texture::CreateObject()
{
    GLuint tex = 0;
	glGenTextures(1, &tex);
	if(0 == tex)
	{
		throw std::runtime_error("Can't create gl texture.");
	}
	return tex;
}

void Texture::DeleteObject(GLuint obj)
{
	glDeleteTextures(1, &obj);
}

void Texture::TexImage(const void* image, GLsizei length, GLenum format, GLsizei width, GLsizei height)
{
	glBindTexture(m_Type, m_Obj);
	glTexImage2D(m_Type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	bool mipmap = (0 == (width & (width - 1)) && 0 == (height & (height - 1)));
	GLenum minf;
	GLenum wrap;
	if(mipmap)
	{
		glGenerateMipmap(m_Type);
		minf = GL_LINEAR_MIPMAP_LINEAR;
		wrap = GL_REPEAT;
	}
	else
	{
		minf = GL_LINEAR;
		wrap = GL_CLAMP_TO_EDGE;
	}
	glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, minf);
	glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, wrap);
	
    m_Type = GL_TEXTURE_2D;
	m_Width = width;
	m_Height = height;
}

bool Texture::Load(const char* file)
{
    fipImage image;
    if(!image.load(file))
    {
        return false;
    }
    
    int bpp = image.getBitsPerPixel();
    GLenum format;
    switch(image.getColorType())
    {
    case FIC_RGB:
        if(24 != bpp)
        {
            if(!image.convertTo24Bits())
            {
                return false;
            }
            bpp = 24;
        }
        format = GL_RGB;
        break;
    case FIC_RGBALPHA:
        if(32 != bpp)
        {
            if(!image.convertTo32Bits())
            {
                return false;
            }
            bpp = 32;
        }
        format = GL_RGBA;
        break;
    default:
        return false;
    }

    int width = image.getWidth();    
    int height = image.getHeight();
    int scanline = image.getScanWidth();
    uint8_t* rgb = new uint8_t[height * scanline];
    uint8_t* bgr = image.accessPixels();
    
    if(24 == bpp)
    {
        for(int y = 0;y < height;++y)
        {
            uint8_t* src = bgr + (height - 1 - y) * scanline;
            uint8_t* dst = rgb + y * scanline;
            for(int x = 0;x < width;++x)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                
                dst += 3;
                src += 3;            
            }
        }
    }
    else
    {
        for(int y = 0;y < height;++y)
        {
            uint8_t* src = bgr + (height - 1 - y) * scanline;
            uint8_t* dst = rgb + y * scanline;
            for(int x = 0;x < width;++x)
            {
                dst[0] = src[2];
                dst[1] = src[1];
                dst[2] = src[0];
                dst[3] = src[3];
                
                dst += 4;
                src += 4;            
            }
        }
    }
    
    TexImage(rgb, 0, format, width, height);

    delete[] rgb;

	return true;
}
