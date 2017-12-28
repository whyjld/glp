#include <stdexcept>
#include "vertexarray.h"

VertexArray::VertexArray()
 : GLObj(CreateObject())
{
}

VertexArray::VertexArray(const VertexArray& v)
 : GLObj(v)
{
}
	
VertexArray::VertexArray(VertexArray&& v)
 : GLObj(v)
{
}

VertexArray::~VertexArray()
{
}

void VertexArray::Bind()
{
	glBindVertexArray(m_Obj);
}

void VertexArray::VertexAttrib(GLuint index, GLuint size, GLenum type, GLboolean normalized, GLsizei stride, GLsizei offset)
{
    glVertexAttribPointer(index, size, type, normalized, stride, ((uint8_t*)nullptr) + offset);
    glEnableVertexAttribArray(index);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

GLuint VertexArray::CreateObject()
{
    GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	if(0 == vao)
	{
		throw std::runtime_error("Can't create gl vertex array.");
	}
	return vao;
}

void VertexArray::DeleteObject(GLuint obj)
{
	glDeleteVertexArrays(1, &obj);
}
