#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include "../ShaderProgram.h"
#include "../../Entities/Components/Lightning/PointLight.h"
#include "../../Entities/Components/Lightning/DirectionalLight.h"

#include "glm.hpp"

class Camera;

class PhongLS : public ShaderProgram
{
public:
	PhongLS();
	virtual ~PhongLS();

	void updateUniforms(GLuint* textures, unsigned nrOf);
	void updateLights();

	void addPointLight(PointLight::PointLightData* data);
	void setDirectionalLight(DirectionalLight::DirectionalLightData* data);

	void setShadowCamera(Camera* shadowCamera);
	void setShadowSize(const glm::vec2& shadowSize);

private:
	void selfUpdateUniforms(Node* entity);

	std::vector<PointLight::PointLightData*> pointLights;
	DirectionalLight::DirectionalLightData* directionalLight;

	GLuint ubo, bindingPoint;

	Camera* shadowCamera;
};

#endif