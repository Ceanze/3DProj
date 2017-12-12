#ifndef DISPLAY_H
#define DISPLAY_H

#include "Config.h"

#include <string>

class Display
{
public:
	Display(const std::string& title);
	Display(const std::string& title, unsigned int width, unsigned int height);
	virtual ~Display();

	void updateView(unsigned int width, unsigned int height);

	unsigned int getWidth() const;
	unsigned int getHeight() const;
	std::string getTitle() const;
	GLFWwindow* getWindowPtr() const;
	float getRatio() const;

public:
	static Display* displayPtr;
	bool shouldUpdateSize;

private:
	void init();
	static void resizeCallback(GLFWwindow* window, int width, int height);

private:
	unsigned int width;
	unsigned int height;
	std::string title;
	GLFWwindow* window;

};

#endif
