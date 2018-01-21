#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "..\Lightning\LightComponent.h"

#include "glm.hpp"



class PointLight : public LightComponent
{
public:
	struct PointLightData
	{
		glm::vec4 positionRadius;
		glm::vec4 colorIntensity;
	};
public:
	PointLight(const float& radius, const float& intensity, const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f), PhongLS* phongShader = nullptr);
	virtual ~PointLight();

	void init();
	void update(const float& dt);

private:
	PointLightData data;
	float radius;
};

#endif