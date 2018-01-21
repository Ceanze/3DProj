#ifndef GEOMETRY_NOMRAL_MAP_SHADER_H
#define GEOMETRY_NOMRAL_MAP_SHADER_H

#include "../ShaderProgram.h"

class GeometryNormalMapShader : public ShaderProgram
{
public:
	GeometryNormalMapShader();
	virtual ~GeometryNormalMapShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);

private:
	void selfUpdateUniforms(Node* node);

	glm::mat4 proj;
	glm::mat4 view;
};

#endif