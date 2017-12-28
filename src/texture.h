#ifndef _TEXTURE_h_
#define _TEXTURE_h_

#include <string>
#include "globj.h"

class Texture : public GLObj<GLuint>
{
public:
	explicit Texture(const char* file);
	explicit Texture(GLenum type = GL_TEXTURE_2D);
	Texture(const Texture& v);	
	Texture(Texture&& v);

	~Texture();

	Texture& operator=(const Texture& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint>::operator=(v);
	        m_Type = v.m_Type;
	        m_Width = v.m_Width;
	        m_Height = v.m_Height;
	    }
	    return *this;
	}
	
	Texture& operator=(Texture&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint>::operator=(v);
	        m_Type = v.m_Type;
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
protected:
    GLuint CreateObject();
    virtual void DeleteObject(GLuint obj);
private:
    GLenum m_Type;
	GLsizei m_Width;
	GLsizei m_Height;
};

#endif//_TEXTURE_h_