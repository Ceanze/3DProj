#ifndef ENGINECORE_H
#define ENGINECORE_H


#include <glfw3.h>
#include <glm.hpp>

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	void init();

private:
	void update(const float& dt);
	void render();

	GLFWwindow* window;
};

#endif // !ENGINECORE_H
