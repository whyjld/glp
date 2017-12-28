#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include <decode.h>
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
	
	m_Width = width;
	m_Height = height;
}

bool Texture::Load(const char* file)
{
	GLenum format;

	uint8_t* image = nullptr;
	
	enum TexType
	{
		ttNone,
		ttTGA,
		ttWebP,
	};
	TexType texType = ttNone;
	GLsizei width = 0;
	GLsizei height = 0;
	if(nullptr != strstr(file, ".tga"))
	{
		if(!LoadTGA(file, width, height, format, image))
		{
			return false;;
		}
		texType = ttTGA;
	}
	else if(nullptr != strstr(file, ".webp"))
	{
		std::ifstream inf(file, std::ios::binary | std::ios::in);
		if(!inf)
		{
			return false;
		}
		inf.seekg(0, std::ios::end);
		std::vector<uint8_t> buf(inf.tellg());
		inf.seekg(0, std::ios::beg);
		inf.read((char*)&buf[0], buf.size());
		if((size_t)inf.gcount() != buf.size())
		{
			return false;
		}
		int w, h;
		image = WebPDecodeRGBA(&buf[0], buf.size(), &w, &h);
		width = w;
		height = h;
		format = GL_RGBA;
		
		texType = ttWebP;
	}
	
	if(nullptr == image || ttNone == texType)
	{
        return false;
	}

    TexImage(image, 0, format, width, height);
    m_Type = texType;

	switch(texType)
	{
		case ttTGA:
			delete[] image;
			break;
		case ttWebP:
			WebPFree(image);
			break;
		case ttNone:
		default:
			break;
	};
	return true;
}
