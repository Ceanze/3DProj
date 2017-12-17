#include "ShaderProgram.h"

#include "../Error.h"

ShaderProgram::ShaderProgram()
{
	this->id = glCreateProgram();
}

ShaderProgram::ShaderProgram(const Shader & vertexShader, const Shader & fragmentShader)
{
	this->id = glCreateProgram();
	addShader(vertexShader);
	addShader(fragmentShader);
	link();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(this->id);
}

void ShaderProgram::addShader(const Shader & shader)
{
	this->shaderIds.push_back(shader.getID());
	glAttachShader(this->id, shader.getID());
}

void ShaderProgram::link(bool shouldDeleteShaders)
{
	glLinkProgram(this->id);

	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = GL_FALSE;
	glGetProgramiv(this->id, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		memset(buff, 0, 1024);
		glGetProgramInfoLog(this->id, 1024, nullptr, buff);
		buff[1023] = '/0';
		Error::printError(buff);
	}

	for (unsigned int i = 0; i < this->shaderIds.size(); i++)
	{
		glDetachShader(this->id, shaderIds[i]);
		if (shouldDeleteShaders)
			glDeleteShader(shaderIds[i]);
	}
}

const GLuint ShaderProgram::getID() const
{
	return this->id;
}
