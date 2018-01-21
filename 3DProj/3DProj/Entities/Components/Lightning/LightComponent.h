#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "..\Component.h"

#include <vector>

class PhongLS;

class LightComponent : public Component
{
public:
	LightComponent(PhongLS* phongShader);
	virtual ~LightComponent();

	virtual void init();
	virtual void update(const float& dt);
	virtual void input(Display* display);

	PhongLS* getShader();

private:
	static PhongLS* phongShader;
};

#endif