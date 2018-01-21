#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "../ShaderProgram.h"
#include "../../Entities/Components/Lightning/LightComponent.h"

#include "glm.hpp"


class PhongLS : public ShaderProgram
{
public:
	PhongLS();
	virtual ~PhongLS();

	void updateUniforms(GLuint* textures, unsigned nrOf);

	std::vector<LightComponent*>* getLights();
private:
	void selfUpdateUniforms(Node* entity);

	std::vector<LightComponent*> lights;

	GLuint ssbo;
	GLint positionLoc, normalLoc, albedoLoc, depthLoc, camLoc;
};

#endif