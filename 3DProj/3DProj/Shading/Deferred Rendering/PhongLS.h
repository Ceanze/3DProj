#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "../ShaderProgram.h"

class PhongLS : public ShaderProgram
{
public:
	PhongLS();
	virtual ~PhongLS();

	void updateUniforms(GLuint* textures, unsigned nrOf);
private:
	void selfUpdateUniforms(Node* entity);

	GLint positionLoc, normalLoc, albedoLoc, depthLoc, camLoc;
};

#endif