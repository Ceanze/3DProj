#ifndef GEOMETRY_SHADER_H
#define GEOMETRY_SHADER_H

#include "../ShaderProgram.h"

#include <glm.hpp>

class GeometryShader : public ShaderProgram
{
public:
	GeometryShader();
	virtual ~GeometryShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);
	void setUseWireframe(bool state);
	bool isUsingWireframe() const;

private:
	void selfUpdateUniforms(Node* node);

	glm::mat4 proj;
	glm::mat4 view;
	bool useWireframe;
};

#endif