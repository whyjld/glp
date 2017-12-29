#ifndef _BUFFER_h_
#define _BUFFER_h_

#include <string>
#include "globj.h"

class Buffer : public GLObj<GLuint, Buffer>
{
public:
	Buffer(const void* buf, size_t len, GLenum type, GLenum usage = GL_STATIC_DRAW);
	Buffer(const Buffer& v);	
	Buffer(Buffer&& v);

	~Buffer();

	Buffer& operator=(const Buffer& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Buffer>::operator=(v);
	        m_Type = v.m_Type;
	    }
	    return *this;
	}
	
	Buffer& operator=(Buffer&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Buffer>::operator=(v);
	        m_Type = v.m_Type;
	    }
	    return *this;
	}
	
	void Bind();

    static void DeleteObject(GLuint obj);
private:
    GLuint CreateObject();
    GLenum m_Type;
};

#endif//_BUFFER_h_