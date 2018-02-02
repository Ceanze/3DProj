#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "..\Lightning\LightComponent.h"

#include "glm.hpp"

class DirectionalLight : public LightComponent
{
public:
	struct DirectionalLightData
	{
		glm::vec4 direction;
		glm::vec4 colorIntensity;
	};
public:
	DirectionalLight(const glm::vec3& direction, const float& intensity, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), PhongLS* phongShader = nullptr);
	virtual ~DirectionalLight();

	void init();
	void update(const float& dt);

private:
	DirectionalLightData data;
	float radius;
};

#endif