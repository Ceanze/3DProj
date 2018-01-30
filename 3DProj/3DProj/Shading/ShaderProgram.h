#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <vector>

#include "Shader.h"


#include "../Entities/Node.h"
#include "../Entities/Components/Camera/Camera.h"
#include "../Core/GL Utils/FrameBuffer.h"


class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const Shader& computeShader);
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader);
	ShaderProgram(const Shader& vertexShader, const Shader& fragmentShader, const Shader& geometryShader);
	virtual ~ShaderProgram();

	void addShader(const Shader& shader);
	void link(bool shouldDeleteShaders = true);

	void updateUniforms();
	void updateUniforms(Node* entity);

	const GLuint getID() const;

	void setCamera(Camera* camera);
	Camera* getCamera() { return this->camera; };

	virtual void updateUniforms(GLuint* textures, unsigned nrOf) = 0;

private:
	virtual void selfUpdateUniforms(Node* entity) = 0;

	Camera* camera;
	GLuint id;
	std::vector<GLuint> shaderIds;
};

#endif
