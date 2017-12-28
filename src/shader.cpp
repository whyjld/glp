#include <stdexcept>
#include <iostream>
#include <vector>
#include <webp/decode.h>
#include "shader.h"
#include "tga.h"

Shader::Shader(const char* source, size_t len, GLenum type)
 : GLObj(glCreateShader(type))
{
	if (0 == m_Obj)
	{
		throw std::runtime_error("Can't create shader.");
	}
    ShaderSource(source, len);   
}

Shader::Shader(const char* file, GLenum type)
 : GLObj(glCreateShader(type))
{
	if (0 == m_Obj)
	{
		throw std::runtime_error("Can't create shader.");
	}
	LoadShader(file);
}
	
Shader::Shader(Shader&& v)
 : GLObj(v)
{
}

Shader::~Shader()
{
}

void Shader::DeleteObject(GLuint obj)
{
	glDeleteShader(obj);
}

void Shader::LoadShader(const char* file)
{
    std::ifstream inf(file);
    if(!inf)
    {
        throw std::runtime_error("Can't open shader file.");
    }
    inf.seekg(0, std::ios::end);
    std::vector<char> buf(inf.tellg());
    inf.seekg(0, std::ios::beg);
    inf.read((char*)&buf[0], buf.size());
    if((size_t)inf.gcount() != buf.size())
    {
        throw std::runtime_error("Can't load shader file.");
    }
    ShaderSource(&buf[0], buf.size());
}

void Shader::ShaderSource(const char* source, size_t len)
{
    if(0 == len)
    {
	    glShaderSource(m_Obj, 1, &source, nullptr);
	}
	else
	{
        GLint length = len;
	    glShaderSource(m_Obj, 1, &source, &length);
	}
    glCompileShader(m_Obj);

	GLint isCompiled = 0;
	glGetShaderiv(m_Obj, GL_COMPILE_STATUS, &isCompiled);

	if (0 == isCompiled)
	{
	    std::string msg = "Can't compile shader.\nSource:\n";
		msg += source;
		GLint logLen = 0;

		glGetShaderiv(m_Obj, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 1)
		{
			std::string log;
			log.resize(logLen);
			glGetShaderInfoLog(m_Obj, logLen, &logLen, (GLchar*)(log.c_str()));
			msg += "\nError:";
			msg += log;
		}
		throw std::runtime_error(msg.c_str());
	}
}