#include <stdexcept>
#include "renderbuffer.h"

Renderbuffer::Renderbuffer(GLenum format, GLsizei width, GLsizei height)
 : GLObj(CreateObject())
 , m_Format(format)
 , m_Width(width)
 , m_Height(height)
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Obj);
    glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

Renderbuffer::Renderbuffer(const Renderbuffer& v)
 : GLObj(v)
 , m_Format(v.m_Format)
 , m_Width(v.m_Width)
 , m_Height(v.m_Height)
{
}
	
Renderbuffer::Renderbuffer(Renderbuffer&& v)
 : GLObj(v)
 , m_Format(v.m_Format)
 , m_Width(v.m_Width)
 , m_Height(v.m_Height)
{
}

Renderbuffer::~Renderbuffer()
{
}

void Renderbuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, m_Obj);
}

GLuint Renderbuffer::CreateObject()
{
    GLuint buf = 0;
	glGenRenderbuffers(1, &buf);
	if(0 == buf)
	{
		throw std::runtime_error("Can't create renderbuffer.");
	}
	return buf;
}

void Renderbuffer::DeleteObject(GLuint obj)
{
	glDeleteRenderbuffers(1, &obj);
}
