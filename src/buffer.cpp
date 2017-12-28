#include <stdexcept>
#include "buffer.h"

Buffer::Buffer(const void* buf, size_t len, GLenum type, GLenum usage)
 : GLObj(CreateObject())
 , m_Type(type)
{
	glBindBuffer(type, m_Obj);
    glBufferData(type, len, buf, usage);
}

Buffer::Buffer(const Buffer& v)
 : GLObj(v)
 , m_Type(v.m_Type)
{
}
	
Buffer::Buffer(Buffer&& v)
 : GLObj(v)
 , m_Type(v.m_Type)
{
}

Buffer::~Buffer()
{
}

void Buffer::Bind()
{
	glBindBuffer(m_Type, m_Obj);
}

GLuint Buffer::CreateObject()
{
    GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	if(0 == vbo)
	{
		throw std::runtime_error("Can't create gl Buffer.");
	}
	return vbo;
}

void Buffer::DeleteObject(GLuint obj)
{
	glDeleteBuffers(1, &obj);
}
