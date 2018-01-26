#include "EngineCore.h"

#include <chrono>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"
#include "..\Entities\Components\Movement\testComponent.h"
#include "..\Entities\Components\Lightning\PointLight.h"
#include "../Terrain/Terrain.h"

#include "ResourceManager.h"

/*---------------- TEMP --------------------*/
#include <gtc\matrix_transform.hpp>
/*-------------- END TEMP ------------------*/

EngineCore::EngineCore()
	:	display("test window"),
		terrain(64, 1)
{
	/*---------------- TEMP --------------------*/
	// Create Shader
	//this->testShader = new TestShader();
	this->geometryShader = new GeometryShader();
	this->geometryNMShader = new GeometryNormalMapShader();
	this->deferredRenderer = new DeferredRenderer(&this->display);

	this->camera = new Camera(&this->display, glm::vec3{0.0f, 5.0f, 10.0f});
	this->geometryShader->setCamera(this->camera);
	this->geometryNMShader->setCamera(this->camera);
	this->deferredRenderer->setCamera(this->camera);
	//this->testShader->setCamera(this->camera);
	
	this->terrain.setShader(this->geometryShader);

	this->base = new Entity({ 0.0f, 5.0f, -5.0f }, {0.0f, 0.0f, 0.0f});

	/*this->m1 = new Mesh();
	loader.load(this->m1, "Bunny/bunny.obj");*/
	this->m2 = new Mesh();
	loader.load(this->m2, "Cube/Cube.obj", USE_NORMAL_MAP);

	loader.load(this->cubeMeshes, "Cube2/Cube2.obj", USE_NORMAL_MAP);
	loader.load(this->armyPilotMeshes, "ArmyPilot/ArmyPilot.obj", FLIP_UV_Y);
	loader.load(this->swordMeshes, "Sword2a/sword2a.obj");
	
	// --------------------------- Bunny ---------------------------
	/*this->e1 = new Entity({ -3.0f, 1.f, -5.0f }, glm::normalize(glm::vec3{ 0.1f, 2.0f, -2.0f }), false);
	this->e1->addMesh(this->m1, this->geometryShader);
	this->e1->addComponent(new testComponent());
	
	
	base->addChild(e1);
*/
	// --------------------------- Bunny and Cube ---------------------------
	this->e2 = new Entity({ 3.0f, -1.f, -5.0f }, { 0.0f, 0.0f, 0.0f }, false);
	//this->e2->addMesh(this->m1, this->geometryShader);
	this->e2->addMeshes(this->cubeMeshes, this->geometryNMShader);
	this->e2->addComponent(this->camera);
	base->addChild(e2);
	
	// --------------------------- Sword ---------------------------
	this->sword = new Entity({ 0.0f, -2.0f, -1.0f }, { 0.0f, 0.0f, 0.0f }, false);
	this->sword->getLocalTransform().setScale({ 2.0f, 2.0f, 2.0f });
	this->sword->getLocalTransform().setRotation({ 0.0f, 0.0f, -3.1415f / 2.0f });
	this->sword->addMeshes(this->swordMeshes, this->geometryShader);
	base->addChild(sword);

	// --------------------------- Army pilot ---------------------------
	this->armyPilot = new Entity({ 0.0f, -5.f, 5.0f }, { 0.0f, 0.0f, 0.0f }, false);
	this->armyPilot->getLocalTransform().setScale({0.05f, 0.05f, 0.05f });
	this->armyPilot->addMeshes(this->armyPilotMeshes, this->geometryShader);
	//this->armyPilot->addComponent(new PointLight(40.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f), this->deferredRenderer->getPhongShader()));
	base->addChild(armyPilot);

	// --------------------------- Arm ---------------------------
	Entity* temp = new Entity({ 0.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryNMShader);
	base->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryNMShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryNMShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryNMShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	// ------------------------- Lights ---------------------------
	this->lightBase = new Entity({ 0.0f, 6.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	this->base->addChild(this->lightBase);

	temp = new Entity({ -4.0f, 0.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f), this->deferredRenderer->getPhongShader()));
	temp->addMesh(this->m2, this->geometryNMShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);


	temp = new Entity({ 4.0f, 0.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), this->deferredRenderer->getPhongShader()));
	temp->addMesh(this->m2, this->geometryNMShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);


	temp = new Entity({ -4.0f, 0.0f, 6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), this->deferredRenderer->getPhongShader()));
	temp->addMesh(this->m2, this->geometryNMShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);

	this->base->init();
	/*-------------- END TEMP ------------------*/
}

EngineCore::~EngineCore()
{
	//delete this->testShader;
	//delete this->phongShader;
	//delete this->m1;
	delete this->m2;
	for (Mesh* m : this->cubeMeshes) delete m;
	for (Mesh* m : this->armyPilotMeshes) delete m;
	for (Mesh* m : this->swordMeshes) delete m;
	delete this->base;

	delete this->geometryNMShader;
	delete this->geometryShader;
	delete this->deferredRenderer;

	ResourceManager::deleteResources();
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
			this->deferredRenderer->resize(&this->display);
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

	/*Transform& tb = this->base->getWorldTransform();
	tb.setRotation(tb.getRotation() + glm::vec3{ 0.0f, -dt, 0.0f });
	
	Transform& a1 = this->arm[1]->getWorldTransform();
	a1.setRotation(a1.getRotation() + glm::vec3{ dt, 0.0f, dt });
	Transform& a2 = this->arm[2]->getLocalTransform();
	a2.setRotation(a2.getRotation() + glm::vec3{ -dt, 0.0f, dt });
	*/
	/*Transform& t = this->e1->getWorldTransform();
	t.setRotation(t.getRotation() + glm::vec3{ dt*1.5f, dt*1.5f, dt*1.5f });
	*/

	Transform& lightT = this->lightBase->getWorldTransform();
	lightT.setRotation(lightT.getRotation() + glm::vec3{ 0.0f, -dt, 0.0f });

	this->base->update(dt);
	
}

void EngineCore::render()
{
	this->display.bindAsRenderTarget();
	// Draw
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->deferredRenderer->render(this->base, &this->terrain);

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
	{
		if (ImGui::Button("Node tree Window")) show_node_tree_window ^= 1;
		if (ImGui::Button("Deferred Rendering")) show_dr_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Click 'C' to toggle camera on and off.");
		ImGui::Text("Click 'B' to toggle blur on and off.");
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
		renderLSTextures();
		ImGui::End();
	}
}

void EngineCore::renderNodeGUI(Node* e, int level)
{
	if (ImGui::TreeNode("Transform"))
	{
		Transform& t = e->getWorldTransform();
		glm::vec3 pos = t.getTranslation();
		ImGui::DragFloat3("Position", &pos[0], 0.01f, -100.0f, 100.0f);
		t.setTranslation(pos);

		glm::vec3 rot = t.getRotation();
		ImGui::DragFloat3("Rotation", &rot[0], 0.01f, 0.0f, 2 * 3.1415f);
		t.setRotation(rot);

		glm::vec3 scale = t.getScale();
		ImGui::DragFloat3("Scale", &scale[0], 0.01f, 0.01f, 100.0f);
		t.setScale(scale);
		ImGui::TreePop();
	}

	std::vector<Mesh*> meshes = static_cast<Entity*>(e)->getMeshes();
	if (meshes.size() != 0)
	{
		if (ImGui::TreeNode("Textures"))
		{
			ImGui::Text("Albedo map");
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				Texture* texture = meshes[i]->material->texture;
				ImTextureID texID = (ImTextureID)texture->getTexture();
				float ratio = (float)texture->getWidth() / (float)texture->getHeight();
				renderTexture(texID, ratio, i != meshes.size() - 1);
			}

			ImGui::Text("Normal map");
			for (unsigned int i = 0; i < meshes.size(); i++)
			{
				Texture* texture = meshes[i]->material->normalMap;
				if (texture != nullptr)
				{
					ImTextureID texID = (ImTextureID)texture->getTexture();
					float ratio = (float)texture->getWidth() / (float)texture->getHeight();
					renderTexture(texID, ratio, i != meshes.size() - 1);
				}
			}

			ImGui::TreePop();
		}
	}

	std::vector<Node*>& nodes = e->getChildren();
	for (unsigned int i = 0; i < nodes.size(); i++)
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
	if (ImGui::TreeNode("GBuffer"))
	{
		const FrameBuffer* gBuffer = this->deferredRenderer->getGBuffer();
		for (unsigned i = 0; i < gBuffer->getNumTextures(); i++)
		{
			ImTextureID texID = (ImTextureID)gBuffer->getTexture(i);
			float ratio = (float)gBuffer->getWidth() / (float)gBuffer->getHeight();
			renderTexture(texID, ratio, i != gBuffer->getNumTextures() - 1);
		}
		ImGui::TreePop();
	}
}

void EngineCore::renderLSTextures()
{
	if (ImGui::TreeNode("LightningBuffer"))
	{
		const FrameBuffer* lBuffer = this->deferredRenderer->getLBuffer();
		for (unsigned i = 0; i < lBuffer->getNumTextures(); i++)
		{
			ImTextureID texID = (ImTextureID)lBuffer->getTexture(i);
			float ratio = (float)lBuffer->getWidth() / (float)lBuffer->getHeight();
			renderTexture(texID, ratio, i != lBuffer->getNumTextures() - 1);
		}
		ImGui::TreePop();
	}
}

void EngineCore::renderTexture(ImTextureID texID, float ratio, bool nextLine)
{
	ImGui::Image(texID, ImVec2(50 * ratio, 50), ImVec2(0, 1), ImVec2(1, 0));
	if (nextLine)
		ImGui::SameLine();
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::Image(texID, ImVec2(170 * ratio, 170), ImVec2(0, 1), ImVec2(1, 0), ImVec4(1, 1, 1, 1), ImVec4(1, 1, 1, 1));
		ImGui::EndTooltip();
	}
}

