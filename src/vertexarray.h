#ifndef _VERTEXARRAY_h_
#define _VERTEXARRAY_h_

#include <string>
#include "globj.h"

class VertexArray : public GLObj<GLuint>
{
public:
	VertexArray();
	VertexArray(const VertexArray& v);	
	VertexArray(VertexArray&& v);

	~VertexArray();

	VertexArray& operator=(const VertexArray& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint>::operator=(v);
	    }
	    return *this;
	}
	
	VertexArray& operator=(VertexArray&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint>::operator=(v);
	    }
	    return *this;
	}
	
	void Bind();
	
	void VertexAttrib(GLuint index, GLuint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset); 
	
	void Unbind();
protected:
    GLuint CreateObject();
    virtual void DeleteObject(GLuint obj);
private:
};

#endif//_VERTEXARRAY_h_