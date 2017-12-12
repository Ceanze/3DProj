#include <iostream>
#include <Windows.h>

#include "Display.h"

int main(void)
{
	Display display("Test window");
	
	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{
		// Draw
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Swap buffers
		glfwSwapBuffers(display.getWindowPtr());
		glfwPollEvents();
	}
	return 0;
}