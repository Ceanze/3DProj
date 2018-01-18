#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "../ShaderProgram.h"

class PhongLS : public ShaderProgram
{
public:
	PhongLS();
	virtual ~PhongLS();

	void updateUniforms(FrameBuffer* buffer);
private:
	void selfUpdateUniforms(Node* entity);

	GLuint positionLoc, normalLoc, albedoLoc, depthLoc;
};

#endif