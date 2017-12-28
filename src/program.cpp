#include <stdexcept>
#include "program.h"

Program::Program(const std::vector<Shader>& shaders)
 : Program(&shaders[0], shaders.size())
{
}

Program::Program(const Shader* shaders, size_t count)
 : GLObj(glCreateProgram())
{
	if (0 == m_Obj)
	{
		throw std::runtime_error("Can't create program.");
	}
	for(size_t i = 0;i < count;++i)
	{
	    glAttachShader(m_Obj, shaders[i].getGLObj());
	}
	glLinkProgram(m_Obj);

	GLint isLinked = 0;
	glGetProgramiv(m_Obj, GL_LINK_STATUS, &isLinked);

	if (0 == isLinked)
	{
	    std::string msg = "Can't link program.";
	    
		GLint logLen = 0;

		glGetProgramiv(m_Obj, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 1)
		{
			std::string log;
			log.resize(logLen);
			glGetProgramInfoLog(m_Obj, logLen, &logLen, (GLchar*)(log.c_str()));
			msg += "Error:";
			msg += log;
		}
		throw std::runtime_error(msg.c_str());
	}
}

Program::Program(const Program& v)
 : GLObj(v)
{
}
	
Program::Program(Program&& v)
 : GLObj(v)
{
}

Program::~Program()
{
}

void Program::Use() const
{
    glUseProgram(m_Obj);
}

bool Program::setUniform(const char* name, float v)
{
    auto i = m_UniformIndices.find(name);
    if(i != m_UniformIndices.end())
    {
        glUniform1f(i->second, v);
        return true;
    }
    else
    {
        return false;
    }
}

bool Program::setUniform(const char* name, const glm::vec2& v)
{
    auto i = m_UniformIndices.find(name);
    if(i != m_UniformIndices.end())
    {
        glUniform2fv(i->second, 1, &v[0]);
        return true;
    }
    else
    {
        return false;
    }
}

bool Program::setUniform(const char* name, const glm::vec3& v)
{
    auto i = m_UniformIndices.find(name);
    if(i != m_UniformIndices.end())
    {
        glUniform3fv(i->second, 1, &v[0]);
        return true;
    }
    else
    {
        return false;
    }
}

bool Program::setUniform(const char* name, const glm::vec4& v)
{
    auto i = m_UniformIndices.find(name);
    if(i != m_UniformIndices.end())
    {
        glUniform4fv(i->second, 1, &v[0]);
        return true;
    }
    else
    {
        return false;
    }
}

bool Program::setUniform(const char* name, const glm::mat4& v)
{
    auto i = m_UniformIndices.find(name);
    if(i != m_UniformIndices.end())
    {
        glUniformMatrix4fv(i->second, 1, false, &v[0][0]);
        return true;
    }
    else
    {
        return false;
    }
}

void Program::DeleteObject(GLuint obj)
{
    glDeleteProgram(obj);
}

