#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "../ShaderProgram.h"
#include "../../Entities/Components/Lightning/PointLight.h"

#include "glm.hpp"


class PhongLS : public ShaderProgram
{
public:
	PhongLS();
	virtual ~PhongLS();

	void updateUniforms(GLuint* textures, unsigned nrOf);
	void updateLights();

	void addPointLight(PointLight::PointLightData* data);
private:
	void selfUpdateUniforms(Node* entity);

	std::vector<PointLight::PointLightData*> pointLights;

	GLuint ubo, bindingPoint;
	GLint positionLoc, normalLoc, kdALoc, ksNsLoc, camLoc, nrOf;
};

#endif