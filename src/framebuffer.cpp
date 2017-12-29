#include <stdexcept>
#include "framebuffer.h"

Framebuffer::Framebuffer()
 : GLObj(CreateObject())
{
}

Framebuffer::Framebuffer(const Framebuffer& v)
 : GLObj(v)
{
}
	
Framebuffer::Framebuffer(Framebuffer&& v)
 : GLObj(v)
{
}

Framebuffer::~Framebuffer()
{
}

void Framebuffer::Bind(GLenum target)
{
	glBindTexture(target, m_Obj);
}

void Framebuffer::Unbind(GLenum target)
{
    glBindTexture(target, 0);
}

void Framebuffer::Attache(GLenum target, GLenum attachment, Texture&& tex)
{
    glFramebufferTexture(target, attachment, tex.getGLObj(), 0);
}

void Framebuffer::Attache(GLenum target, GLenum attachment, Renderbuffer&& buf)
{
    glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, buf.getGLObj());
}

bool Framebuffer::CheckStatus()
{
    return (GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus(GL_FRAMEBUFFER));
}

GLuint Framebuffer::CreateObject()
{
    GLuint tex = 0;
	glGenFramebuffers(1, &tex);
	if(0 == tex)
	{
		throw std::runtime_error("Can't create gl framebuffer.");
	}
	return tex;
}

void Framebuffer::DeleteObject(GLuint obj)
{
	glDeleteFramebuffers(1, &obj);
}
