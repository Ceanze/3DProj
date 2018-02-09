#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <glew.h>
#include <vector>
#include <unordered_map>

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

	void setTexture2D(const std::string& name, unsigned int unit);
	void setTexture2D(const std::string& name, unsigned int unit, GLuint textureID);
	void setUniform1i(const std::string& name, int value);
	void setUniform1b(const std::string& name, bool value);
	void setUniform2f(const std::string& name, float v1, float v2);
	void setUniform2fv(const std::string& name, unsigned int count, const float* values);
	void setUniform3f(const std::string& name, float v1, float v2, float v3);
	void setUniform3fv(const std::string& name, unsigned int count, const float* values);
	void setUniform4f(const std::string& name, float v1, float v2, float v3, float v4);
	void setUniform4fv(const std::string& name, unsigned int count, const float* values);
	void setUniformMatrix4fv(const std::string& name, unsigned int count, bool transpose, const float* values);

	void updateUniforms();
	void updateUniforms(Node* entity);

	const GLuint getID() const;

	void setCamera(Camera* camera);
	Camera* getCamera() { return this->camera; };

	virtual void updateUniforms(GLuint* textures, unsigned nrOf) = 0;

private:
	int addUniform(const std::string& name);
	virtual void selfUpdateUniforms(Node* entity) = 0;

	Camera* camera;
	GLuint id;
	std::unordered_map<std::string, GLint> uniforms;
	std::vector<GLuint> shaderIds;
	std::string name;
};

#endif
