#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <vector>

#include "Shader.h"

#include "../Entities/Node.h"

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
	virtual ~ShaderProgram();

	void addShader(const Shader& shader);
	void link(bool shouldDeleteShaders = true);

	virtual void updateUniforms(Node* entity) {};

	const GLuint getID() const;
private:
	GLuint id;
	std::vector<GLuint> shaderIds;
};

#endif
