#include "EngineCore.h"

#include <chrono>

/*---------------- TEMP --------------------*/
#include <gtc\matrix_transform.hpp>
/*-------------- END TEMP ------------------*/

EngineCore::EngineCore()
	: display("test window")
{
	/*---------------- TEMP --------------------*/
	// Load and construct Meshes
	this->mesh = new Mesh();
	this->mesh2 = new Mesh();
	this->loader.load(this->mesh, "Cube/Cube.obj");
	this->loader.load(this->mesh2, "Bunny/bunny.obj");

	// Load shader.
	this->vsShader = new Shader("Phong/Phong.vs", GL_VERTEX_SHADER);
	this->fsShader = new Shader("Phong/Phong.fs", GL_FRAGMENT_SHADER);
	this->sp = new ShaderProgram(*this->vsShader, *this->fsShader);

	// Load data from mesh to GPU.
	this->mesh->loadToGPU(this->sp->getID());
	this->mesh2->loadToGPU(this->sp->getID());

	// Create each model.
	this->model = new Model(this->mesh, { 2.0f, 0.5f, 0.0f }, glm::normalize(glm::vec3(1.0f, 2.5f, -5.0f))); // Move cube1 to (2.0, 0.5, 0.0) and make it has the direction norm(2.0, 0.5, -5.0)
	this->model2 = new Model(this->mesh, { -2.5f, -0.5f, 0.0f }, glm::normalize(glm::vec3(-2.5f, -0.5f, -5.0f)));// Move cube2 to (-2.5, -0.5, 0.0) and make it has the direction norm(-2.5, -0.5, -5.0)
	this->model3 = new Model(this->mesh2, glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 0.5f})); // Scale down the bunny with a factor of 2.

	// Creat an entity which has all the models.
	this->entity = new Entity(*this->model);
	this->entity->addModel(*this->model2);
	this->entity->addModel(*this->model3);

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
	/*---------------- TEMP --------------------*/
	delete this->mesh;
	delete this->mesh2;
	delete this->sp;
	delete this->model;
	delete this->model2;
	delete this->model3;
	delete this->vsShader;
	delete this->fsShader;
	delete this->entity;
	/*-------------- END TEMP ------------------*/
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
	this->entity->getModel(0).setLocalMatrix(glm::rotate(this->entity->getModel(0).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f })); // Rotate Cube 1
	this->entity->getModel(1).setLocalMatrix(glm::rotate(this->entity->getModel(1).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f })); // Rotate Cube 2
	this->entity->setLocalMatrix(glm::rotate(this->entity->getLocalMatrix(), -dt, {0.0f, 1.0f, 0.0f})); // Rotate Entity (rotate Cube 1 and Cube 2 around Bunny).
	/*-------------- END TEMP ------------------*/
}

void EngineCore::render()
{
	this->display.bindAsRenderTarget();
	// Draw
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*---------------- TEMP --------------------*/
	glUseProgram(this->sp->getID());
	this->entity->draw(this->sp->getID());
	/*-------------- END TEMP ------------------*/

	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

