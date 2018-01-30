#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "..\Component.h"
#include "..\..\..\Core\Display.h"

class Movement : public Component
{
public:
	Movement(float speed = 5, unsigned forward = GLFW_KEY_UP, unsigned left = GLFW_KEY_LEFT, unsigned backward = GLFW_KEY_DOWN, unsigned right = GLFW_KEY_RIGHT);
	~Movement();

	void init();
	void update(const float& dt);
	void input(Display* display);
	
private:
	float speed;
	float dt;
	unsigned forward, left, backward, right;
};

#endif