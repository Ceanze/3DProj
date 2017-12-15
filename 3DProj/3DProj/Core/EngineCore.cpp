#include "EngineCore.h"

EngineCore::EngineCore()
	: display("test window")
{
	
}


EngineCore::~EngineCore()
{
}

void EngineCore::init()
{


	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{
		this->update(1.0f);
		this->render();

		glfwPollEvents();
	}

}

void EngineCore::update(const float & dt)
{
}

void EngineCore::render()
{
	this->display.bindAsRenderTarget();
	// Draw
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

