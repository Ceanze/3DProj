#include "ShaderProgram.h"

#include "../Error.h"

ShaderProgram::ShaderProgram()
{
	this->id = glCreateProgram();
	this->name = "";
}

ShaderProgram::ShaderProgram(const Shader & computeShader)
{
	this->id = glCreateProgram();
	this->name = "";
	addShader(computeShader);
	link();
}

ShaderProgram::ShaderProgram(const Shader & vertexShader, const Shader & fragmentShader)
{
	this->id = glCreateProgram();
	this->name = "";
	addShader(vertexShader);
	addShader(fragmentShader);
	link();
}

ShaderProgram::ShaderProgram(const Shader & vertexShader, const Shader & fragmentShader, const Shader & geometryShader)
{
	this->id = glCreateProgram();
	this->name = "";
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
	std::string sname = "[" + shader.getName() + ": ";
	switch (shader.getType())
	{
	case GL_FRAGMENT_SHADER:	sname += "Fragment shader"; break;
	case GL_GEOMETRY_SHADER:	sname += "Geometry shader | "; break;
	case GL_VERTEX_SHADER:		sname += "Vertex shader | "; break;
	}
	this->name += sname + "]";
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

void ShaderProgram::setTexture2D(const std::string & name, unsigned int unit)
{
	glUniform1i(addUniform(name), unit);
}

void ShaderProgram::setTexture2D(const std::string & name, unsigned int unit, GLuint textureID)
{
	setTexture2D(name, unit);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void ShaderProgram::setUniform1i(const std::string & name, int value)
{
	glUniform1i(addUniform(name), value);
}

void ShaderProgram::setUniform1b(const std::string & name, bool value)
{
	setUniform1i(name, (int)value);
}

void ShaderProgram::setUniform2f(const std::string & name, float v1, float v2)
{
	glUniform2f(addUniform(name), v1, v2);
}

void ShaderProgram::setUniform2fv(const std::string & name, unsigned int count, const float * values)
{
	glUniform2fv(addUniform(name), count, values);
}

void ShaderProgram::setUniform3f(const std::string & name, float v1, float v2, float v3)
{
	glUniform3f(addUniform(name), v1, v2, v3);
}

void ShaderProgram::setUniform3fv(const std::string & name, unsigned int count, const float * values)
{
	glUniform3fv(addUniform(name), count, values);
}

void ShaderProgram::setUniform4f(const std::string & name, float v1, float v2, float v3, float v4)
{
	glUniform4f(addUniform(name), v1, v2, v3, v4);
}

void ShaderProgram::setUniform4fv(const std::string & name, unsigned int count, const float * values)
{
	glUniform4fv(addUniform(name), count, values);
}

void ShaderProgram::setUniformMatrix4fv(const std::string & name, unsigned int count, bool transpose, const float * values)
{
	glUniformMatrix4fv(addUniform(name), count, transpose, values);
}

void ShaderProgram::setCamera(Camera * camera)
{
	this->camera = camera;
}

int ShaderProgram::addUniform(const std::string & name)
{
	if (this->uniforms.find(name) == this->uniforms.end())
	{
		int uniformLocation = glGetUniformLocation(this->id, name.c_str());
		if (uniformLocation == -1)
			Error::printError("Can't find uniform '" + name + "' in shader '" + this->name + "'");
		this->uniforms.insert({ name, uniformLocation });
	}
	return this->uniforms[name];
}

void ShaderProgram::updateUniforms()
{
	setUniformMatrix4fv("camera", 1, false, &(this->camera->getVP())[0][0]);

	glm::mat4 identityMatrix(1.0f);
	setUniformMatrix4fv("wm", 1, false, &identityMatrix[0][0]);
}

void ShaderProgram::updateUniforms(Node * entity)
{
	setUniformMatrix4fv("camera", 1, false, &(this->camera->getVP())[0][0]);

	selfUpdateUniforms(entity);
}

const GLuint ShaderProgram::getID() const
{
	return this->id;
}