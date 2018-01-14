#include "EngineCore.h"

#include <chrono>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"
#include "..\Entities\Components\Movement Components\testComponent.h"

/*---------------- TEMP --------------------*/
#include <gtc\matrix_transform.hpp>
/*-------------- END TEMP ------------------*/

EngineCore::EngineCore()
	:	display("test window")//,
		//stateLoader("Resources/Scenes/scene1.txt", &this->display)
{
	/*---------------- TEMP --------------------*/
	// Load shader.
	//this->vsShader = new Shader("Phong/Phong.vs", GL_VERTEX_SHADER);
	//this->fsShader = new Shader("Phong/Phong.fs", GL_FRAGMENT_SHADER);
	//this->sp = new ShaderProgram(*this->vsShader, *this->fsShader);

	// Create Shader
	this->phongShader = new PhongShader();
	this->testShader = new TestShader();

	this->camera = new Camera(&this->display, glm::vec3{0.0f, 0.0f, 10.0f});
	this->phongShader->setCamera(this->camera);
	this->testShader->setCamera(this->camera);

	this->base = new Entity({ 0.0f, 0.0f, -5.0f }, {1.0f, 0.0f, 0.0f});

	this->m1 = new Mesh();
	loader.load(this->m1, "Bunny/bunny.obj");
	this->m2 = new Mesh();
	loader.load(this->m2, "Cube/Cube.obj");
	
	this->e1 = new Entity({ -3.0f, 1.f, -5.0f }, glm::normalize(glm::vec3{ 0.1f, 2.0f, -2.0f }), false);
	this->e1->addMesh(this->m1, this->phongShader);
	this->e1->addComponent(new testComponent());
	this->e1->addComponent(this->camera);
	base->addChild(e1);

	this->e2 = new Entity({ 3.0f, -1.f, -5.0f }, glm::normalize(glm::vec3{ 2.0f, -0.0f, -1.0f }), false);
	this->e2->addMesh(this->m2, this->testShader);
	this->e2->addMesh(this->m1, this->testShader);
	base->addChild(e2);
	
	Entity* temp = new Entity({ 0.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->testShader);
	base->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->testShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->testShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->testShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	this->base->init();
	
	/*
	for (int i = 0; i < this->stateLoader.getMeshes()->size(); i++)
	{
		(*this->stateLoader.getMeshes())[i]->loadToGPU(this->sp->getID());
	}*/

	// Construct a simple camera.
	/*glUseProgram(sp->getID());
	GLuint camLoc = glGetUniformLocation(this->sp->getID(), "camera");
	if (camLoc == -1) fprintf(stderr, "Can't find 'camera' in the shader!");
	glm::mat4 proj = glm::perspective(glm::pi<float>() / 4.0f, this->display.getRatio(), 0.1f, 100.0f);
	glm::mat4 cam = proj*glm::lookAt(glm::vec3( 0.0f, 0.0f, 10.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(camLoc, 1, GL_FALSE, &cam[0][0]);*/
	/*-------------- END TEMP ------------------*/
}

EngineCore::~EngineCore()
{
	/*delete this->vsShader;
	delete this->fsShader;
	delete this->sp;*/
	delete this->testShader;
	delete this->phongShader;
	delete this->m1;
	delete this->m2;
	delete this->base;
}

void EngineCore::init()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto previousTime = currentTime;
	float dt = 0.0f;

	// ------------ GUI TEST VARIABLES --------------
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// ----------------------------------------------

	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{
		if (this->display.sizeUpdated)
		{
			this->camera->updateProj();
			this->display.sizeUpdated = false;
		}

		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		
		// ------------------------------- GUI TEST ---------------------------------
		renderGui();
		// ------------------------------------------------------------------------


		// Compute deltat time (dt)
		currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float>(currentTime-previousTime).count();
		previousTime = currentTime;

		this->input(&this->display);
		this->update(dt);
		this->render();
	}

}

void EngineCore::update(const float & dt)
{
	/*---------------- TEMP --------------------*/
	//this->entity->getModel(0).setLocalMatrix(glm::rotate(this->entity->getModel(0).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f })); // Rotate Cube 1
	//this->entity->getModel(1).setLocalMatrix(glm::rotate(this->entity->getModel(1).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f })); // Rotate Cube 2
	//this->entity->setLocalMatrix(glm::rotate(this->entity->getLocalMatrix(), -dt, {0.0f, 1.0f, 0.0f})); // Rotate Entity (rotate Cube 1 and Cube 2 around Bunny).
	/*-------------- END TEMP ------------------*/

	/*
	if (this->stateLoader.getEntities()->size() > 0)
	{
		Entity * temp = (*this->stateLoader.getEntities())[0];
		//temp->getModel(0).setLocalMatrix(glm::rotate(temp->getModel(0).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f }));
		//temp->getModel(1).setLocalMatrix(glm::rotate(temp->getModel(1).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f }));
		//temp->setLocalMatrix(glm::rotate(temp->getLocalMatrix(), -dt, { 0.0f, 1.0f, 0.0f }));
	}*/

	float& time = this->testShader->getTime();
	time += dt*3.0f;
	if (time > 10)
		time = 0;
	
	Transform& tb = this->base->getWorldTransform();
	tb.setRotation(tb.getRotation() + glm::vec3{ 0.0f, -dt, 0.0f });
	
	Transform& a1 = this->arm[1]->getWorldTransform();
	a1.setRotation(a1.getRotation() + glm::vec3{ dt, 0.0f, dt });
	Transform& a2 = this->arm[2]->getLocalTransform();
	a2.setRotation(a2.getRotation() + glm::vec3{ -dt, 0.0f, dt });
	
	Transform& t = this->e1->getWorldTransform();
	t.setRotation(t.getRotation() + glm::vec3{ dt*1.5f, dt*1.5f, dt*1.5f });
	
	this->base->update(dt);
	
}

void EngineCore::render()
{
	this->display.bindAsRenderTarget();
	// Draw
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*---------------- TEMP --------------------*/
	this->base->render();

	//this->entity->draw(this->sp->getID());

	//for (int i = 0; i < this->stateLoader.getEntities()->size(); i++)
	//	(*this->stateLoader.getEntities())[i]->render();

	/*-------------- END TEMP ------------------*/

	// Draw ImGui elements.
	ImGui::Render();
	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

void EngineCore::input(Display* display)
{
	base->input(display);
}

void EngineCore::renderGui()
{
	static bool show_node_tree_window = false;
	{
		ImGui::Text("Hello, world!");
		if (ImGui::Button("Node tree Window")) show_node_tree_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
	if (show_node_tree_window)
	{
		ImGui::Begin("Node tree Window", &show_node_tree_window);
		if (ImGui::TreeNode("Base Node level 0"))
		{
			renderNodeGUI(this->base);
			ImGui::TreePop();
		}
		ImGui::End();
	}
}

void EngineCore::renderNodeGUI(Node* e, int level)
{
	Transform& t = e->getWorldTransform();
	glm::vec3 pos = t.getTranslation();
	ImGui::DragFloat3("Position", &pos[0], 0.01f, -100.0f, 100.0f);
	t.setTranslation(pos);

	glm::vec3 rot = t.getRotation();
	ImGui::DragFloat3("Rotation", &rot[0], 0.01f, 0.0f, 2*3.1415f);
	t.setRotation(rot);

	glm::vec3 scale = t.getScale();
	ImGui::DragFloat3("Scale", &scale[0], 0.01f, 0.01f, 100.0f);
	t.setScale(scale);

	std::vector<Node*>& nodes = e->getChildren();
	for (int i = 0; i < nodes.size(); i++)
	{
		std::string nodeName("Node " + std::to_string(i) + " in level " + std::to_string(level));
		if (ImGui::TreeNode(nodeName.c_str()))
		{
			renderNodeGUI(nodes[i], level+1);
			ImGui::TreePop();
		}
	}
}

