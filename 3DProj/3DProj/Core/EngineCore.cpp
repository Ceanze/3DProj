#include "EngineCore.h"

#include <chrono>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"
#include "..\Entities\Components\Movement Components\testComponent.h"

/*---------------- TEMP --------------------*/
#include <gtc\matrix_transform.hpp>
/*-------------- END TEMP ------------------*/

EngineCore::EngineCore()
	:	display("test window")
{
	/*---------------- TEMP --------------------*/
	// Create Shader
	//this->phongShader = new PhongShader();
	//this->testShader = new TestShader();
	this->geometryShader = new GeometryShader();
	this->deferredRenderer = new DeferredRenderer(&this->display);

	this->camera = new Camera(&this->display, glm::vec3{0.0f, 0.0f, 10.0f});
	this->geometryShader->setCamera(this->camera);
	//this->phongShader->setCamera(this->camera);
	//this->testShader->setCamera(this->camera);

	this->base = new Entity({ 0.0f, 0.0f, -5.0f }, {1.0f, 0.0f, 0.0f});

	/*this->m1 = new Mesh();
	loader.load(this->m1, "Bunny/bunny.obj");*/
	this->m2 = new Mesh();
	loader.load(this->m2, "Cube/Cube.obj");
	
	this->e1 = new Entity({ -3.0f, 1.f, -5.0f }, glm::normalize(glm::vec3{ 0.1f, 2.0f, -2.0f }), false);
	this->e1->addMesh(this->m2, this->geometryShader);
	this->e1->addComponent(new testComponent());
	this->e1->addComponent(this->camera);
	base->addChild(e1);

	this->e2 = new Entity({ 3.0f, -1.f, -5.0f }, glm::normalize(glm::vec3{ 2.0f, -0.0f, -1.0f }), false);
	this->e2->addMesh(this->m2, this->geometryShader);
	this->e2->addMesh(this->m2, this->geometryShader);
	base->addChild(e2);
	
	Entity* temp = new Entity({ 0.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->geometryShader);
	base->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 1.0f });
	temp->addMesh(this->m2, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	this->base->init();
	/*-------------- END TEMP ------------------*/
}

EngineCore::~EngineCore()
{
	//delete this->testShader;
	//delete this->phongShader;
	//delete this->m1;
	delete this->m2;
	delete this->base;

	delete this->geometryShader;
	delete this->deferredRenderer;
}

void EngineCore::init()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto previousTime = currentTime;
	float dt = 0.0f;

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
	/*
	float& time = this->testShader->getTime();
	time += dt*3.0f;
	if (time > 10)
		time = 0;
	*/

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

	this->deferredRenderer->render(this->base);

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
	static bool show_dr_window = false;
	static bool show_ls_window = false;
	{
		if (ImGui::Button("Node tree Window")) show_node_tree_window ^= 1;
		if (ImGui::Button("Deferred Rendering")) show_dr_window ^= 1;
		if (ImGui::Button("Lightning Shader")) show_ls_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Click 'C' to toggle camera on and off.");
	}

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

	if (show_dr_window)
	{
		ImGui::Begin("Deferred Rendering Window", &show_dr_window);
		renderDRTextures();
		ImGui::End();
	}

	if (show_ls_window)
	{
		ImGui::Begin("Lightning Shader Window", &show_dr_window);
		renderLSTextures();
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

void EngineCore::renderDRTextures()
{
	if (ImGui::TreeNode("Deferred Rendering"))
	{
		if (ImGui::TreeNode("GBuffer"))
		{
			const FrameBuffer* gBuffer = this->deferredRenderer->getGBuffer();
			for (unsigned i = 0; i < gBuffer->getNumTextures(); i++)
			{
				ImTextureID texID = (ImTextureID)gBuffer->getTexture(i);
				float ratio = (float)gBuffer->getWidth() / (float)gBuffer->getHeight();
				ImGui::Image(texID, ImVec2(50 * ratio, 50), ImVec2(0, 1), ImVec2(1, 0));
				if (i != gBuffer->getNumTextures() - 1)
					ImGui::SameLine();
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image(texID, ImVec2(170* ratio, 170), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
					ImGui::EndTooltip();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

void EngineCore::renderLSTextures()
{
	if (ImGui::TreeNode("Lightning Shader"))
	{
		if (ImGui::TreeNode("LightningBuffer"))
		{
			const FrameBuffer* lBuffer = this->deferredRenderer->getLBuffer();
			for (unsigned i = 0; i < lBuffer->getNumTextures(); i++)
			{
				ImTextureID texID = (ImTextureID)lBuffer->getTexture(i);
				float ratio = (float)lBuffer->getWidth() / (float)lBuffer->getHeight();
				ImGui::Image(texID, ImVec2(50 * ratio, 50), ImVec2(0, 1), ImVec2(1, 0));
				if (i != lBuffer->getNumTextures() - 1)
					ImGui::SameLine();
				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::Image(texID, ImVec2(170 * ratio, 170), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
					ImGui::EndTooltip();
				}
			}
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
}

