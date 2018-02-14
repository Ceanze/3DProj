#include "Display.h"

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"

Display* Display::displayPtr = nullptr;

Display::Display(const std::string & title)
{
	this->title = title;
	this->width = DEFAULT_WIDTH;
	this->height = DEFAULT_HEIGHT;
	init();
}

Display::Display(const std::string & title, unsigned int width, unsigned int height)
{
	this->title = title;
	this->width = width;
	this->height = height;
	init();
}

Display::~Display()
{
#ifdef RENDER_GUI
	ImGui_ImplGlfwGL3_Shutdown();
#endif
	glfwTerminate();
	glfwDestroyWindow(this->window);
}

void Display::resizeCallback(GLFWwindow * window, int width, int height)
{
	if (width != 0 || height != 0)
	{
		Display::displayPtr->updateView(width, height);
		Display::displayPtr->sizeUpdated = true;
	}
}

void Display::updateView(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	glViewport(0, 0, this->width, this->height);
}

void Display::bindAsRenderTarget()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0, 0, getWidth(), getHeight());
}

unsigned int Display::getWidth() const
{
	return this->width;
}

unsigned int Display::getHeight() const
{
	return this->height;
}

std::string Display::getTitle() const
{
	return this->title;
}

GLFWwindow * Display::getWindowPtr() const
{
	return this->window;
}

float Display::getRatio() const
{
	return (float)this->width / (float)this->height;
}

void Display::init()
{
	Display::displayPtr = this;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW!");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4);// 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Modern opengl
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Make MacOS happy
#endif

	window = glfwCreateWindow(this->width, this->height, this->title.c_str(), NULL, NULL);
	if (window == NULL)
	{
		fprintf(stderr, "Failed to open GLFW window!");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	// --------------- INIT GLEW ---------------
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW!");
		exit(EXIT_FAILURE);
	}

	glViewport(0, 0, this->width, this->height);

	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glfwSetWindowSizeCallback(this->window, resizeCallback);

	// Setup ImGui binding
#ifdef RENDER_GUI
	ImGui_ImplGlfwGL3_Init(window, true);
#endif
}

