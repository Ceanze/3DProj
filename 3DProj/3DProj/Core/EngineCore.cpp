#include "EngineCore.h"

#include <chrono>

/*---------------- TEMP --------------------*/
#include <gtc\matrix_transform.hpp>
/*-------------- END TEMP ------------------*/

EngineCore::EngineCore()
	:	display("test window"),
		stateLoader("Resources/Scenes/scene1.txt", &this->display)
{
	/*---------------- TEMP --------------------*/
	// Load shader.
	this->vsShader = new Shader("Phong/Phong.vs", GL_VERTEX_SHADER);
	this->fsShader = new Shader("Phong/Phong.fs", GL_FRAGMENT_SHADER);
	this->sp = new ShaderProgram(*this->vsShader, *this->fsShader);

	for (int i = 0; i < this->stateLoader.getMeshes()->size(); i++)
	{
		(*this->stateLoader.getMeshes())[i]->loadToGPU(this->sp->getID());
	}

	// Construct a simple camera.
	glUseProgram(sp->getID());
	GLuint camLoc = glGetUniformLocation(this->sp->getID(), "camera");
	if (camLoc == -1) fprintf(stderr, "Can't find 'camera' in the shader!");
	glm::mat4 proj = glm::perspective(glm::pi<float>() / 4.0f, this->display.getRatio(), 0.1f, 100.0f);
	glm::mat4 cam = proj*glm::lookAt(glm::vec3( 0.0f, 0.0f, 10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(camLoc, 1, GL_FALSE, &cam[0][0]);
	/*-------------- END TEMP ------------------*/
}

EngineCore::~EngineCore()
{

}

void EngineCore::init()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto previousTime = currentTime;
	float dt = 0.0f;

	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{
		// Compute deltat time (dt)
		currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float>(currentTime-previousTime).count();
		previousTime = currentTime;

		this->update(dt);
		this->render();

		glfwPollEvents();
	}

}

void EngineCore::update(const float & dt)
{
	/*---------------- TEMP --------------------*/
	//this->entity->getModel(0).setLocalMatrix(glm::rotate(this->entity->getModel(0).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f })); // Rotate Cube 1
	//this->entity->getModel(1).setLocalMatrix(glm::rotate(this->entity->getModel(1).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f })); // Rotate Cube 2
	//this->entity->setLocalMatrix(glm::rotate(this->entity->getLocalMatrix(), -dt, {0.0f, 1.0f, 0.0f})); // Rotate Entity (rotate Cube 1 and Cube 2 around Bunny).
	/*-------------- END TEMP ------------------*/

	Entity * temp = (*this->stateLoader.getEntities())[0];
	temp->getModel(0).setLocalMatrix(glm::rotate(temp->getModel(0).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f }));
	temp->getModel(1).setLocalMatrix(glm::rotate(temp->getModel(1).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f }));
	temp->setLocalMatrix(glm::rotate(temp->getLocalMatrix(), -dt, { 0.0f, 1.0f, 0.0f }));
	
}

void EngineCore::render()
{
	this->display.bindAsRenderTarget();
	// Draw
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*---------------- TEMP --------------------*/
	glUseProgram(this->sp->getID());
	
	//this->entity->draw(this->sp->getID());

	for (int i = 0; i < this->stateLoader.getEntities()->size(); i++)
		(*this->stateLoader.getEntities())[i]->draw(this->sp->getID());

	/*-------------- END TEMP ------------------*/

	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

