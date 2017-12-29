#ifndef _VERTEXARRAY_h_
#define _VERTEXARRAY_h_

#include <string>
#include "globj.h"

class VertexArray : public GLObj<GLuint, VertexArray>
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
	        GLObj<GLuint, VertexArray>::operator=(v);
	    }
	    return *this;
	}
	
	VertexArray& operator=(VertexArray&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, VertexArray>::operator=(v);
	    }
	    return *this;
	}
	
	void Bind();
	
	void VertexAttrib(GLuint index, GLuint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset); 
	
	void Unbind();

    static void DeleteObject(GLuint obj);
private:
    GLuint CreateObject();
};

#endif//_VERTEXARRAY_h_