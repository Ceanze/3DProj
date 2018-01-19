#ifndef TESTCOMPONENT_H
#define TESTCOMPONENT_H

#include "..\Component.h"

class testComponent : public Component
{
public:
	testComponent(float rotSpeed = 0.0);
	~testComponent();

	void init();
	void update(const float& dt);
	void input(Display* display);

private:
	float rotSpeed;
};

#endif