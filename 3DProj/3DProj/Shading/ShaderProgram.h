#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <vector>

#include "Shader.h"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
	~ShaderProgram();

	void addShader(const Shader& shader);
	void link(bool shouldDeleteShaders = true);

	const GLuint getID() const;
private:
	GLuint id;
	std::vector<GLuint> shaderIds;
};

#endif
