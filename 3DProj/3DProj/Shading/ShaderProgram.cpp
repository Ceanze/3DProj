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

ShaderProgram::ShaderProgram(const Shader & vertexShader, const Shader & fragmentShader, const Shader & geometryShader)
{
	this->id = glCreateProgram();
	addShader(vertexShader);
	addShader(fragmentShader);
	addShader(geometryShader);
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

void ShaderProgram::setCamera(Camera * camera)
{
	this->camera = camera;
}

void ShaderProgram::updateUniforms()
{
	GLuint camLoc = glGetUniformLocation(this->getID(), "camera");
	if (camLoc != -1)
	{
		glUniformMatrix4fv(camLoc, 1, GL_FALSE, &(this->camera->getVP())[0][0]);
	}
	else Error::printError("Could not find 'camera' in shader!");

	glm::mat4 identityMatrix(1.0f);

	GLuint wmLoc = glGetUniformLocation(this->getID(), "wm");
	if (wmLoc != -1)
	{
		glUniformMatrix4fv(wmLoc, 1, GL_FALSE, &identityMatrix[0][0]);
	}
}

void ShaderProgram::updateUniforms(Node * entity)
{
	GLuint camLoc = glGetUniformLocation(this->getID(), "camera");
	if (camLoc != -1)
	{
		glUniformMatrix4fv(camLoc, 1, GL_FALSE, &(this->camera->getVP())[0][0]);
	}
	else Error::printError("Could not find 'camera' in shader!");

	selfUpdateUniforms(entity);
}

const GLuint ShaderProgram::getID() const
{
	return this->id;
}