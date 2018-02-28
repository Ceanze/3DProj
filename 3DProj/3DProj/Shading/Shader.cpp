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
	this->name = name;		//Sets the name (+ extra path if needed)
	this->type = type;		//Sets the type (fragment, vertex, geometry)
	create(name, type);		//Create the shader
	compile();				//Compile the created shader
}

Shader::~Shader()
{
	glDeleteShader(this->id);
}

void Shader::create(const std::string & name, GLuint type)
{
	this->id = glCreateShader(type);								//Create the shader of the type
	std::ifstream file(SHADER_PATH + name);							//Open the shader file
	std::string shaderText;
	if (file.is_open())
	{
		shaderText = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());		//Read from the shader file
		file.close();
	}
	else
	{
		Error::printError("Could not open shader : " + std::string(SHADER_PATH + name));
	}
	const char* shaderTextPtr = shaderText.c_str();			//Make the string into C-style char pointer
	glShaderSource(this->id, 1, &shaderTextPtr, nullptr);	//Give the shader ID the source code from the file
}

void Shader::compile()
{
	char buff[1024];												//Buffer for errors
	memset(buff, 0, 1024);											//Set the bytes to 0
	GLint compileResult = GL_FALSE;
	glCompileShader(this->id);										//Compile the shader
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &compileResult);		//Check if it failed the compilation
	if (compileResult == GL_FALSE) {
		glGetShaderInfoLog(this->id, 1024, nullptr, buff);
		buff[1023] = (char)'/0';
		Error::printError(buff);									//Print error message if fail
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
