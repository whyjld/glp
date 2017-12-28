#ifndef _PROGRAM_h_
#define _PROGRAM_h_

#include "globj.h"
#include "shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <unordered_map>
#include <vector>
#include <string>

class Program : public GLObj<GLuint>
{
public:
	Program(const std::vector<Shader>& shaders);
	Program(const Shader* shaders, size_t count);
	Program(const Program& v);	
	Program(Program&& v);

	~Program();

	Program& operator=(const Program& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint>::operator=(v);
	        m_UniformIndices = v.m_UniformIndices;
	    }
	    return *this;
	}
	
	Program& operator=(Program&& v)
	{
	    if(this != &v)
	    {
	        GLObj<GLuint>::operator=(v);
	        m_UniformIndices = v.m_UniformIndices;
	    }
	    return *this;
	}
	
	void Use() const;

	bool setUniform(const char* name, float v);
	bool setUniform(const char* name, const glm::vec2& v);
	bool setUniform(const char* name, const glm::vec3& v);
	bool setUniform(const char* name, const glm::vec4& v);
	
	bool setUniform(const char* name, const glm::mat4& v);
protected:
    virtual void DeleteObject(GLuint obj);
private:
    std::unordered_map<std::string, GLuint> m_UniformIndices;
};

#endif//_PROGRAM_h_