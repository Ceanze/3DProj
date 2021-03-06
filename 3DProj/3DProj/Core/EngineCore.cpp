#include "EngineCore.h"

#include <chrono> //Deltatime

#include "../ImGui/imgui.h" //UI
#include "../ImGui/imgui_impl_glfw_gl3.h" //UI
#include "..\Entities\Components\Movement\Movement.h" //Movement
#include "..\Entities\Components\Lightning\PointLight.h" //Pointlight
#include "../Utils/Utils.h" //Used for calculations of yaw, pitch, roll.
#include "../Terrain/Terrain.h" //Terrain

#include "ResourceManager.h"

EngineCore::EngineCore()
	:	display("test window"),
		terrain()
{
	// ----------------------- Shaders ----------------------------
	this->geometryShader = new GeometryShader();
	this->deferredRenderer = new DeferredRenderer(&this->display);

	// ----------------------- Cameras ----------------------------
	const glm::vec3 lightDir = glm::normalize(-glm::vec3(0.0f, 8.0f, 5.0f));								//Used for shadowCamera and directional light
	this->shadowCamera = new Camera(&this->display, 50, 50, lightDir, {0.0f, 0.0f, 0.0f}, -100, 200);		//Create the shadowCamera
	this->deferredRenderer->setShadowCamera(this->shadowCamera);											//Send the shadowCamera to the deferred renderer
	this->camera = new Camera(&this->display, glm::vec3{0.0f, 13.5f, 0.0f});								//Create the perspecive camera (player)
	this->activeCamera = this->camera;																		//Sets the current active camera to the perspective camera
	this->camInc = 0;																						//Used to swtich between cameras
	attachCamera(this->activeCamera);																		//Attach the active camera to the two shaders
	this->frustum = new Frustum(this->activeCamera, this->terrain.getQuadTree(), this->display.getRatio()); //Create a frustum for perspective camera with reference to the quadTree
	
	// ---------------------- Terrain -----------------------------
	this->terrain.setShader(this->geometryShader);															//Set the shader for the terrain

	// --------------------- Multi-use mesh -----------------------
	loader.load(this->cubeMeshes, "Cube/Cube.obj", USE_NORMAL_MAP);											//Load a mesh to the object
	this->cubeMeshes[0]->material->glowColor = glm::vec3(1.0f, 0.5f, 1.0f);									//Set a glowColor for the material

	// --------------------- Load the meshes ----------------------
	loader.load(this->cube2Meshes, "Cube2/Cube2.obj", USE_NORMAL_MAP);
	loader.load(this->armyPilotMeshes, "ArmyPilot/ArmyPilot.obj", FLIP_UV_Y);
	loader.load(this->knightMeshes, "Knight/knight.obj", FLIP_UV_Y);
	loader.load(this->dragonMeshes, "Dragon/dragon.obj");

	// Base node for the node system. Every node is being moved with reference to this. Every entity is a node
	this->base = new Entity({ 0.0f, 10.0f, -5.0f }, {0.0f, 0.0f, 0.0f});

	// --------------------------- Player ---------------------------
	this->player = new Entity({ 0.0f, 3.0f, 15.0f }, { 0.0f, 0.0f, 0.0f }, false);
	this->player->addComponent(this->camera);																	 //Adding the camera to the player entity
	this->player->addComponent(new Movement(&this->terrain, 10, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D));//Player entity can now move with WASD
	base->addChild(player);																						 //Add the entity to the base node

	this->directionalLight = new Entity({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });						 //Create the directional light
	this->directionalLight->addComponent(this->shadowCamera);												 //shadowCamera is a component and is added to directionalLight
	
	this->directionalLight->addComponent(new DirectionalLight(lightDir, 2.0f, glm::vec3(1.0f), this->deferredRenderer->getPhongShader())); //Add the light to the entity
	this->player->addChild(this->directionalLight);

	// --------------------------- Knight ---------------------------
	Entity* temp2 = new Entity({ 8.0f, -5.f, 5.0f }, { 0.0f, 0.0f, 0.0f }, false);
	temp2->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	temp2->addMeshes(this->knightMeshes, this->geometryShader);
	base->addChild(temp2);

	// --------------------------- Dragon ---------------------------
	Entity* temp3 = new Entity({ -10.0f, 0.0f, 15.0f }, { 0.0f, 0.0f, 0.0f }, false);
	temp3->getLocalTransform().setScale({ 0.5f, 0.5f, 0.5f });
	temp3->addMeshes(this->dragonMeshes, this->geometryShader);
	base->addChild(temp3);

	// --------------------------- Army pilot ---------------------------
	this->armyPilot = new Entity({ 0.0f, -5.f, 5.0f }, { 0.0f, 0.0f, 0.0f }, false);
	this->armyPilot->getLocalTransform().setScale({0.05f, 0.05f, 0.05f });
	this->armyPilot->addMeshes(this->armyPilotMeshes, this->geometryShader);
	base->addChild(armyPilot);

	// --------------------------- Arm ---------------------------
	Entity* temp = new Entity({ 0.0f, -3.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	base->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	temp = new Entity({ 0.0f, 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addMeshes(this->cube2Meshes, this->geometryShader);
	this->arm[this->arm.size() - 1]->addChild(temp);
	this->arm.push_back(temp);

	// ------------------------- Lights ---------------------------
	this->lightBase = new Entity({ 0.0f, 2.0f, -20.0f }, { 0.0f, 0.0f, 0.0f });
	this->base->addChild(this->lightBase);
	
	temp = new Entity({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f), this->deferredRenderer->getPhongShader()));
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);
	
	temp = new Entity({ 4.0f, 0.0f, -6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f), this->deferredRenderer->getPhongShader()));
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);

	temp = new Entity({ -4.0f, 0.0f, 6.0f }, { 0.0f, 0.0f, 0.0f });
	temp->addComponent(new PointLight(50.0f, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f), this->deferredRenderer->getPhongShader()));
	temp->addMeshes(this->cubeMeshes, this->geometryShader);
	temp->getLocalTransform().setScale({ 0.2f, 0.2f, 0.2f });
	this->lightBase->addChild(temp);
	
	this->base->init();		//init all the nodes
	this->frustum->init();	//Init the frustum
}

EngineCore::~EngineCore()
{
	for (Mesh* m : this->cubeMeshes) delete m;
	for (Mesh* m : this->cube2Meshes) delete m;
	for (Mesh* m : this->armyPilotMeshes) delete m;
	for (Mesh* m : this->dragonMeshes) delete m;
	for (Mesh* m : this->knightMeshes) delete m;
	delete this->base;
	delete this->frustum;

	delete this->geometryShader;
	delete this->deferredRenderer;

	ResourceManager::deleteResources();
}

void EngineCore::init()
{
	// -------------------- Delta time -----------------------
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto previousTime = currentTime;
	float dt = 0.0f;
	float timePassed = 0.0;

	// -------------------- Main loop ------------------------
	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{
		if (this->display.sizeUpdated)
		{
			this->camera->updateProj();
			this->deferredRenderer->resize(&this->display);
			this->frustum->resize(this->display);
			this->display.sizeUpdated = false;
		}

		glfwPollEvents();
#ifdef RENDER_GUI
		ImGui_ImplGlfwGL3_NewFrame();
#endif
		// ------------------------------- GUI TEST ---------------------------------
#ifdef RENDER_GUI
		renderGui();							//If the gui is selected to show
#else
		timePassed += dt;						//If the gui isn't being renderd, show FPS in console
		if (timePassed > 1.0f)
		{
			printf("FPS: %f\n", 1.0f / dt);
			timePassed = 0.0f;
		}
#endif
		// ------------------------------------------------------------------------

		// Compute delta time (dt)
		currentTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float>(currentTime-previousTime).count();
		previousTime = currentTime;

		this->input(&this->display); //Check if there are any inputs
		this->update(dt); //Uppdate all the entities before rendering
		this->render(); //Renders
	}

}

void EngineCore::update(const float & dt)
{
	this->frustum->update(this->camera->getPosition());									//Update frustum with new position

	Transform& a1 = this->arm[1]->getWorldTransform();
	a1.setRotation(a1.getRotation() + glm::vec3{ dt, 0.0f, dt });
	Transform& a2 = this->arm[2]->getLocalTransform();
	a2.setRotation(a2.getRotation() + glm::vec3{ -dt, 0.0f, dt });
	
	Transform& lightT = this->lightBase->getWorldTransform();
	lightT.setRotation(lightT.getRotation() + glm::vec3{ 0.0f, -dt, 0.0f });
	
	this->base->update(dt);
	
}

void EngineCore::render()
{
	this->display.bindAsRenderTarget(); //Render to current display
	// Draw
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->deferredRenderer->render(this->base, &this->terrain, this->geometryShader->isUsingWireframe());

	// Draw ImGui elements.
#ifdef RENDER_GUI
	ImGui::Render();
#endif
	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

void EngineCore::input(Display* display)
{
	// --------------------- Toggle for F -----------------------------
	static bool isFClicked = 0;
	static bool isFPressed = false;
	if (glfwGetKey(display->getWindowPtr(), GLFW_KEY_F) != GLFW_PRESS)
	{
		if (isFPressed)
		{
			isFPressed = false;
			isFClicked ^= 1;
			this->geometryShader->setUseWireframe(isFClicked);
		}
	}
	else isFPressed = true;

	// -------------------- Toggle for V -----------------------------
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

	//Go through the bases and all it's children inputs.
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
		ImGui::Text("Click 'F' to toggle wireframe on and off.");
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
		this->activeCamera = this->shadowCamera;
		break;
	case 1:
		this->activeCamera = this->camera;
		break;
	}
	this->camInc++;
	if (this->camInc > 1)
		this->camInc = 0;
	attachCamera(this->activeCamera);
}

