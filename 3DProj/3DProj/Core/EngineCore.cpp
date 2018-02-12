#include "EngineCore.h"

#include <chrono>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"
#include "..\Entities\Components\Movement\Movement.h"
#include "..\Entities\Components\Lightning\PointLight.h"
#include "../Utils/Utils.h"
#include "../Terrain/Terrain.h"

#include "ResourceManager.h"

/*---------------- TEMP --------------------*/
#include <gtc\matrix_transform.hpp>
/*-------------- END TEMP ------------------*/

EngineCore::EngineCore()
	:	display("test window"),
		terrain()
{
	/*---------------- TEMP --------------------*/
	// Create Shader
	this->geometryShader = new GeometryShader();
	this->deferredRenderer = new DeferredRenderer(&this->display);

	this->shadowCamera = new Camera(&this->display, 50, 50, {0.0f, 0.0f, 0.0f}, -100, 200);
	this->deferredRenderer->setShadowCamera(this->shadowCamera);
	this->camera = new Camera(&this->display, glm::vec3{0.0f, 0.0f, 0.0f});
	this->camera2 = new Camera(&this->display, glm::vec3{0.0f, 10.0f, 0.0f});
	this->activeCamera = this->camera;
	this->camInc = 0;
	attachCamera(this->activeCamera);
	this->frustum = new Frustum(this->activeCamera, this->terrain.getQuadTree(), this->display.getRatio());
	
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
	this->e2 = new Entity({ 0.0f, 3.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, false);
	//this->e2->addMesh(this->m1, this->geometryShader);
	this->e2->addComponent(this->camera);
	this->e2->addComponent(new Movement(&this->terrain, 10, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D));
	base->addChild(e2);

	this->directionalLight = new Entity({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	this->directionalLight->addComponent(this->shadowCamera);
	const glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -2.0f, 0.0f));
	this->shadowCamera->setDirection(lightDir);
	this->shadowCamera->rotate(Tools::getYaw(lightDir)+ 3.1415f / 2.0f, Tools::getPitch(lightDir), 0.0f);
	this->directionalLight->addComponent(new DirectionalLight(lightDir, 2.0f, glm::vec3(1.0f), this->deferredRenderer->getPhongShader()));
	this->e2->addChild(this->directionalLight);

	
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
	this->armyPilot->addComponent(this->camera2);
	//this->armyPilot->addComponent(new Movement(10));
	base->addChild(armyPilot);

	// --------------------------- Arm ---------------------------
	Entity* temp = new Entity({ 0.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryShader);
	base->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMesh(this->m2, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	//temp->addComponent(this->camera2);
	//temp->addComponent(new Movement(10, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D));
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	// ------------------------- Lights ---------------------------
	this->lightBase = new Entity({ 0.0f, 2.0f, 40.0f }, { 0.0f, 0.0f, 0.0f });
	this->base->addChild(this->lightBase);
	
	temp = new Entity({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), this->deferredRenderer->getPhongShader()));
	temp->addMesh(this->m2, this->geometryShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);

	/*
	temp = new Entity({ 4.0f, 0.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), this->deferredRenderer->getPhongShader()));
	temp->addMesh(this->m2, this->geometryNMShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);


	temp = new Entity({ -4.0f, 0.0f, 6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), this->deferredRenderer->getPhongShader()));
	temp->addMesh(this->m2, this->geometryNMShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);*/

	this->base->update(0.16f);
	this->base->init();
	this->frustum->init();
	this->shadowCamera->setDirection(lightDir);
	/*-------------- END TEMP ------------------*/
}

EngineCore::~EngineCore()
{
	//delete this->m1;
	delete this->m2;
	for (Mesh* m : this->cubeMeshes) delete m;
	for (Mesh* m : this->armyPilotMeshes) delete m;
	for (Mesh* m : this->swordMeshes) delete m;
	delete this->base;
	delete this->frustum;

	delete this->geometryShader;
	delete this->deferredRenderer;

	ResourceManager::deleteResources();
}

void EngineCore::init()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto previousTime = currentTime;
	float dt = 0.0f;
	float timePassed = 0.0;

	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{

		if (this->display.sizeUpdated)
		{
			this->camera->updateProj();
			this->camera2->updateProj();
			this->deferredRenderer->resize(&this->display);
			this->display.sizeUpdated = false;
		}

		glfwPollEvents();
#ifdef RENDER_GUI
		ImGui_ImplGlfwGL3_NewFrame();
#endif
		// ------------------------------- GUI TEST ---------------------------------
#ifdef RENDER_GUI
		renderGui();
#else
		timePassed += dt;
		if (timePassed > 1.0f)
		{
			printf("FPS: %f\n", 1.0f / dt);
			timePassed = 0.0f;
		}
#endif
		// ------------------------------------------------------------------------

		// Compute deltat time (dt)
		currentTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float>(currentTime-previousTime).count();
		previousTime = currentTime;

		this->input(&this->display);
		this->update(dt);
		this->render();
	}

}

void EngineCore::update(const float & dt)
{
	this->frustum->update(this->camera->getPosition());

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
	/*
	DirectionalLight* dirLight = dynamic_cast<DirectionalLight*>(this->directionalLight->getComponent(1));
	glm::vec3 newDir = glm::normalize(glm::rotate(glm::mat4(1.0f), dt*0.1f, { 0.0f, 0.0f, 1.0f })*glm::vec4(dirLight->getDirection(), 0.0f));
	dirLight->setDirection(newDir);
	this->shadowCamera->rotate(Tools::getYaw(newDir) + 3.1415f / 2.0f, Tools::getPitch(newDir), 0.0f);
	*/
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
#ifdef RENDER_GUI
	ImGui::Render();
#endif
	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

void EngineCore::input(Display* display)
{
	static bool isVClicked = 0;
	static bool isVPressed = false;
	if (glfwGetKey(display->getWindowPtr(), GLFW_KEY_V) != GLFW_PRESS)
	{
		if (isVPressed)
		{
			isVPressed = false;
			isVClicked ^= 1;
		}
	}
	else isVPressed = true;

	if (isVClicked)
	{
		swapCamera();
		isVClicked ^= 1;
	}

	base->input(display);
}

void EngineCore::renderGui()
{
	static bool show_frustum_window = false;
	static bool show_node_tree_window = false;
	static bool show_dr_window = false;
	{
		if (ImGui::Button("Node tree Window")) show_node_tree_window ^= 1;
		if (ImGui::Button("Deferred Rendering")) show_dr_window ^= 1;
		if (ImGui::Button("Frustum Window")) show_frustum_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Click 'C' to toggle camera on and off and 'V' to swap camera");
		ImGui::Text("Click 'B' to toggle blur on and off.");
		ImGui::Text("Click 'G' to toggle glow on and off.");
	}

	if (show_node_tree_window)
	{
		ImGui::Begin("Node tree Window", &show_node_tree_window);
		if (ImGui::TreeNode("Base Node level 0"))
		{
			if (ImGui::TreeNode("Active Camera"))
			{
				glm::vec3 pos = this->activeCamera->getPosition();
				ImGui::DragFloat3("Position", &pos[0], 0.01f, -100.0f, 100.0f);
				glm::vec3 dir = this->activeCamera->getDirection();
				ImGui::DragFloat3("Direction", &dir[0], 0.01f, -100.0f, 100.0f);
				float fov = this->activeCamera->getFOV();
				ImGui::DragFloat("fov", &fov, 0.01f, -100.0f, 100.0f);
				ImGui::TreePop();
			}
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
		renderBrightnessTextures();
		renderBlurTextures();
		ImGui::End();
	}

	if (show_frustum_window)
	{
		ImGui::Begin("Frustum Window", &show_frustum_window);
		renderFrustumGUI();
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

		Transform& t2 = e->getLocalTransform();
		glm::vec3 rot = t2.getRotation();
		ImGui::DragFloat3("Rotation", &rot[0], 0.01f, 0.0f, 2 * 3.1415f);
		t2.setRotation(rot);

		glm::vec3 scale = t2.getScale();
		ImGui::DragFloat3("Scale", &scale[0], 0.01f, 0.01f, 100.0f);
		t2.setScale(scale);
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

	if (ImGui::TreeNode("ShadowBuffer"))
	{
		const FrameBuffer* shadowBuffer = this->deferredRenderer->getShadowBuffer();
		for (unsigned i = 0; i < shadowBuffer->getNumTextures(); i++)
		{
			ImTextureID texID = (ImTextureID)shadowBuffer->getTexture(i);
			float ratio = (float)shadowBuffer->getWidth() / (float)shadowBuffer->getHeight();
			renderTexture(texID, ratio, i != shadowBuffer->getNumTextures() - 1);
		}
		ImGui::TreePop();
	}
}

void EngineCore::renderLSTextures()
{
	if (ImGui::TreeNode("LightingBuffer"))
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

void EngineCore::renderBrightnessTextures()
{
	if (ImGui::TreeNode("BrightnessBuffer"))
	{
		const FrameBuffer* brightnessBuffer = this->deferredRenderer->getBrightnessBuffer();
		for (unsigned i = 0; i < brightnessBuffer->getNumTextures(); i++)
		{
			ImTextureID texID = (ImTextureID)brightnessBuffer->getTexture(i);
			float ratio = (float)brightnessBuffer->getWidth() / (float)brightnessBuffer->getHeight();
			renderTexture(texID, ratio, i != brightnessBuffer->getNumTextures() - 1);
		}
		ImGui::TreePop();
	}
}

void EngineCore::renderBlurTextures()
{
	if (ImGui::TreeNode("BlurBuffer"))
	{
		const FrameBuffer* blurBuffer = this->deferredRenderer->getBlurBuffer();
		for (unsigned i = 0; i < blurBuffer->getNumTextures(); i++)
		{
			ImTextureID texID = (ImTextureID)blurBuffer->getTexture(i);
			float ratio = (float)blurBuffer->getWidth() / (float)blurBuffer->getHeight();
			renderTexture(texID, ratio, i != blurBuffer->getNumTextures() - 1);
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

void EngineCore::renderFrustumGUI()
{
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
	ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
	if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
	if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
	draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(50, 50, 50), ImColor(50, 50, 60), ImColor(60, 60, 70), ImColor(50, 50, 60));
	draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255, 255, 255));

	// clip lines within the canvas (if we resize it, etc.)
	draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), true);
	
	QuadTree* tree = this->terrain.getQuadTree();
	glm::vec3 offset(canvas_size.x/2+ canvas_pos.x, 0, canvas_size.y/2+ canvas_pos.y);
	renderQuadGUI(&draw_list, offset, tree);
	//draw_list->AddCircleFilled(ImVec2(camPos.x, camPos.y), 1.5f, ImColor(255, 255, 255), 8);

	glm::vec3 nearCenter = this->camera->getPosition() + this->camera->getDirection() * this->frustum->getZNear();
	glm::vec3 farCenter = this->camera->getPosition() + this->camera->getDirection() * this->frustum->getZFar();

	glm::vec3 nru = nearCenter + this->camera->getUp() * (this->frustum->getNearHeight() / 2) + this->camera->getRight() * (this->frustum->getNearWidth() / 2);
	glm::vec3 fru = farCenter + this->camera->getUp() * (this->frustum->getFarHeight() / 2) + this->camera->getRight() * (this->frustum->getFarWidth() / 2);
	glm::vec3 nlu = nearCenter + this->camera->getUp() * (this->frustum->getNearHeight() / 2) - this->camera->getRight() * (this->frustum->getNearWidth() / 2);
	glm::vec3 flu = farCenter + this->camera->getUp() * (this->frustum->getFarHeight() / 2) - this->camera->getRight() * (this->frustum->getFarWidth() / 2);

	glm::vec3 nrd = nearCenter - this->camera->getUp() * (this->frustum->getNearHeight() / 2) + this->camera->getRight() * (this->frustum->getNearWidth() / 2);
	glm::vec3 frd = farCenter - this->camera->getUp() * (this->frustum->getFarHeight() / 2) + this->camera->getRight() * (this->frustum->getFarWidth() / 2);
	glm::vec3 nld = nearCenter - this->camera->getUp() * (this->frustum->getNearHeight() / 2) - this->camera->getRight() * (this->frustum->getNearWidth() / 2);
	glm::vec3 fld = farCenter - this->camera->getUp() * (this->frustum->getFarHeight() / 2) - this->camera->getRight() * (this->frustum->getFarWidth() / 2);

	nru += offset;
	fru += offset;
	nlu += offset;
	flu += offset;

	nrd += offset;
	frd += offset;
	nld += offset;
	fld += offset;
	
	draw_list->AddLine(ImVec2(nrd.x, nrd.z), ImVec2(frd.x, frd.z), ImColor(126, 126, 126));
	draw_list->AddLine(ImVec2(nld.x, nld.z), ImVec2(fld.x, fld.z), ImColor(126, 126, 126));
	draw_list->AddLine(ImVec2(fld.x, fld.z), ImVec2(frd.x, frd.z), ImColor(126, 126, 126));
	draw_list->AddLine(ImVec2(nld.x, nld.z), ImVec2(frd.x, frd.z), ImColor(126, 126, 126));

	draw_list->AddLine(ImVec2(nld.x, nld.z), ImVec2(nlu.x, nlu.z), ImColor(126, 126, 126));
	draw_list->AddLine(ImVec2(nrd.x, nrd.z), ImVec2(nru.x, nru.z), ImColor(126, 126, 126));

	draw_list->AddLine(ImVec2(nru.x, nru.z), ImVec2(fru.x, fru.z), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(nlu.x, nlu.z), ImVec2(flu.x, flu.z), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(flu.x, flu.z), ImVec2(fru.x, fru.z), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(nlu.x, nlu.z), ImVec2(fru.x, fru.z), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(fld.x, fld.z), ImVec2(flu.x, flu.z), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(frd.x, frd.z), ImVec2(fru.x, fru.z), ImColor(255, 255, 255));

	draw_list->PopClipRect();
}

void EngineCore::renderQuadGUI(ImDrawList** draw_list, glm::vec3 offset, QuadTree* quad)
{
	if (quad->hasChildren)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			if (!quad->children[i]->hasChildren)
			{
				glm::vec3 min;
				glm::vec3 max;
				AABox box = quad->children[i]->getBox();
				for (unsigned int j = 0; j < 8; j++)
				{
					if (j == 0 || (min.x < box.getPoint(j).x || min.z < box.getPoint(j).z)) min = box.getPoint(j);
					if (j == 0 || (max.x > box.getPoint(j).x || max.z > box.getPoint(j).z)) max = box.getPoint(j);
					(*draw_list)->AddCircleFilled(ImVec2(box.getPoint(j).x + offset.x, box.getPoint(j).z + offset.z), 1.5f, ImColor(255, 255, 255), 8);
				}
				glm::vec2 temp(min.x, min.z);
				min.x = temp.y;
				min.z = temp.x;
				temp.x = max.x;
				temp.y = max.z;
				max.x = temp.y;
				max.z = temp.x;
				min += offset;
				max += offset;
				ImColor color = ImColor(0, 255, 0);
				if (!quad->children[i]->isInFrustum()) color = ImColor(255, 0, 0);
				(*draw_list)->AddRectFilled(ImVec2(min.x, min.z), ImVec2(max.x, max.z), color);
			}
			else
			{
				renderQuadGUI(draw_list, offset, quad->children[i]);
			}
		}
	}
}

void EngineCore::attachCamera(Camera* camera)
{
	camera->activate();
	this->geometryShader->setCamera(camera);
	this->deferredRenderer->setCamera(camera);
}

void EngineCore::swapCamera()
{
	this->activeCamera->deactivate();
	switch (this->camInc)
	{
	case 0:
		this->activeCamera = this->camera2;
		break;
	case 1:
		this->activeCamera = this->shadowCamera;
		break;
	case 2:
		this->activeCamera = this->camera;
		break;
	}
	this->camInc++;
	if (this->camInc > 2)
		this->camInc = 0;
	attachCamera(this->activeCamera);
}

