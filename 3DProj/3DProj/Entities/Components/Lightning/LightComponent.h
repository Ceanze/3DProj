#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "..\Component.h"

#include <vector>

class LightComponent : public Component
{
public:
	LightComponent();
	virtual ~LightComponent();

	virtual void init();
	virtual void update(const float& dt);
	virtual void input(Display* display);

	void setListOfLights(std::vector<LightComponent*>* ptr);

private:
	static std::vector<LightComponent*>* listOfLights;
};

#endif