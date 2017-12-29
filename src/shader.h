#ifndef _SHADER_h_
#define _SHADER_h_

#include <string>
#include "globj.h"

#define SHADER_SOURCE(x) #x

class Shader : public GLObj<GLuint, Shader>
{
public:
	Shader(const char* source, size_t len, GLenum type);
	Shader(const char* file, GLenum type);
	Shader(const Shader& v);	
	Shader(Shader&& v);

	~Shader();

	Shader& operator=(const Shader& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Shader>::operator=(v);
	    }
	    return *this;
	}
	
	Shader& operator=(Shader&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint, Shader>::operator=(v);
	    }
	    return *this;
	}

    static void DeleteObject(GLuint obj);
private:
	void LoadShader(const char* file);
	void ShaderSource(const char* source, size_t len);
};

#endif//_SHADER_h_