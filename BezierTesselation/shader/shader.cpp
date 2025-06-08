#include "shader.hpp"
#include "GL\glew.h"
#include "../error/error.hpp"
#include <iostream>

Shader::Shader(int programId)
{
    this->shader = programId;
}

Shader::~Shader()
{
    if(shader)
	    Call(glDeleteProgram(shader));
}

void Shader::setUniformMat4fv(const std::string& name, const Matrix4x4<float>& matrix)
{
	bind();
	Call(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_TRUE, &matrix[0][0]));
}

void Shader::bind() const
{
	Call(glUseProgram(shader));
}

void Shader::setUniformVec4f(const std::string& name, const Vector4<float>& v)
{
	bind();
	Call(glUniform4f(GetUniformLocation(name), v.x, v.y, v.z, v.w));
}

void Shader::setUniformVec3f(const std::string& name, const Vector4<float>& v)
{
    bind();
    Call(glUniform3f(GetUniformLocation(name), v.x, v.y, v.z));
}

void Shader::setUniformVec2f(const std::string& name, const Vector4<float>& v)
{
    bind();
    Call(glUniform2f(GetUniformLocation(name), v.x, v.y));
}

void Shader::setUniformFloat(const std::string& name, float v)
{
    bind();
    Call(glUniform1f(GetUniformLocation(name), v));
}

void Shader::setUniformInt(const std::string& name, int v)
{
    bind();
    Call(glUniform1i(GetUniformLocation(name), v));
}

void Shader::setUniformBool(const std::string& name, bool v)
{
    bind();
    Call(glUniform1i(GetUniformLocation(name), v));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (locationCache.find(name) != locationCache.end())
        return locationCache[name];
    int location = glGetUniformLocation(shader, name.c_str());
    locationCache[name] = location;
    if (location == -1)
    {
        std::cout << "Error finding location for uniform variable " << name << std::endl;
    }
    return location;
}
