#ifndef QUAD_SHADER_H
#define QUAD_SHADER_H

#include "../ShaderProgram.h"


class QuadShader : public ShaderProgram
{
public:
	QuadShader();
	virtual ~QuadShader();

	void updateUniforms(FrameBuffer* buffer);
private:
	void selfUpdateUniforms(Node* entity);

	GLuint positionLoc, normalLoc, albedoLoc, depthLoc;
};

#endif
