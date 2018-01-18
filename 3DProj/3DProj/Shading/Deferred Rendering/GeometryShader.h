#ifndef GEOMETRY_SHADER_H
#define GEOMETRY_SHADER_H

#include "../ShaderProgram.h"

#include <glm.hpp>

class GeometryShader : public ShaderProgram
{
public:
	GeometryShader();
	virtual ~GeometryShader();

	void updateUniforms(FrameBuffer* buffer);

private:
	void selfUpdateUniforms(Node* node);

	glm::mat4 proj;
	glm::mat4 view;
};

#endif