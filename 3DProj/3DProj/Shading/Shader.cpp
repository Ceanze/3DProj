#include "Shader.h"

#include <fstream>
#include <streambuf>
#include "../Core/Config.h"
#include "../Error.h"

Shader::Shader()
{
	this->name = "";
	this->type = 0;
}

Shader::Shader(const std::string & name, GLuint type)
{
	this->name = name;
	this->type = type;
	create(name, type);
	compile();
}

Shader::~Shader()
{
	glDeleteShader(this->id);
}

void Shader::create(const std::string & name, GLuint type)
{
	this->id = glCreateShader(type);
	std::ifstream file(SHADER_PATH + name);
	std::string shaderText;
	if (file.is_open())
	{
		shaderText = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
	}
	else
	{
		Error::printError("Could not open shader : " + std::string(SHADER_PATH + name));
	}
	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(this->id, 1, &shaderTextPtr, nullptr);
}

void Shader::compile()
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = GL_FALSE;
	glCompileShader(this->id);
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		glGetShaderInfoLog(this->id, 1024, nullptr, buff);
		buff[1023] = '/0';
		Error::printError(buff);
	}
}

const GLuint Shader::getID() const
{
	return this->id;
}

std::string Shader::getName() const
{
	return this->name;
}

GLuint Shader::getType() const
{
	return this->type;
}
