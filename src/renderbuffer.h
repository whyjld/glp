#ifndef _RENDER_BUFFER_h_
#define _RENDER_BUFFER_h_

#include <string>
#include "globj.h"

class Renderbuffer : public GLObj<GLuint, Renderbuffer>
{
public:
	Renderbuffer(GLenum format, GLsizei width, GLsizei height);
	Renderbuffer(const Renderbuffer& v);	
	Renderbuffer(Renderbuffer&& v);

	~Renderbuffer();

	Renderbuffer& operator=(const Renderbuffer& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Renderbuffer>::operator=(v);
	        m_Format = v.m_Format;
	        m_Width = v.m_Width;
	        m_Height = v.m_Height;
	    }
	    return *this;
	}
	
	Renderbuffer& operator=(Renderbuffer&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Renderbuffer>::operator=(v);
	        m_Format = v.m_Format;
	        m_Width = v.m_Width;
	        m_Height = v.m_Height;
	    }
	    return *this;
	}
	
	void Bind();
	void TexImage(const void* image, GLsizei length, GLenum format, GLsizei width, GLsizei height);
	bool Load(const char* file);
	
	GLsizei getWidth() const
	{
	    return m_Width;
	}
	
	GLsizei getHeight() const
	{
	    return m_Height;
	}

    static void DeleteObject(GLuint obj);
protected:
    GLuint CreateObject();
private:
    GLenum m_Format;
	GLsizei m_Width;
	GLsizei m_Height;
};

#endif//_RENDER_BUFFER_h_