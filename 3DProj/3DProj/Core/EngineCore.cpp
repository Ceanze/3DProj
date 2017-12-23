#include "EngineCore.h"

#include <chrono>

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"

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

	// ------------ GUI TEST VARIABLES --------------
	bool show_test_window = true;
	bool show_entities_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// ----------------------------------------------

	glfwSetInputMode(display.getWindowPtr(), GLFW_STICKY_KEYS, GL_TRUE);
	while (glfwGetKey(display.getWindowPtr(), GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(display.getWindowPtr()) == 0)
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();
		
		// ------------------------------- GUI TEST ---------------------------------
		{
			{
				static float f = 0.0f;
				ImGui::Text("Hello, world!");
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("clear color", (float*)&clear_color);
				if (ImGui::Button("Test Window")) show_test_window ^= 1;
				if (ImGui::Button("Entities Window")) show_entities_window ^= 1;
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name the window.
			if (show_entities_window)
			{
				ImGui::Begin("Entities Window", &show_entities_window);
				std::vector<Entity*> entites = *this->stateLoader.getEntities();
				for (int i = 0; i < entites.size(); i++)
				{
					std::string entityName("Entity " + std::to_string(i));
					if (ImGui::TreeNode(entityName.c_str()))
					{
						glm::mat4& wm = entites[i]->getWorldMatrix();
						glm::vec3 pos = { wm[3][0], wm[3][1], wm[3][2] };
						ImGui::DragFloat3("Position", &pos[0], 0.01f, -100.0f, 100.0f);
						wm[3][0] = pos.x;
						wm[3][1] = pos.y;
						wm[3][2] = pos.z;
						std::vector<Model>& models = entites[i]->getModels();
						for (int j = 0; j < models.size(); j++)
						{
							std::string modelName("Model " + std::to_string(j));
							if (ImGui::TreeNode(modelName.c_str()))
							{
								glm::mat4& wm2 = models[j].getWorldMatrix();
								glm::vec3 pos2 = { wm2[3][0], wm2[3][1], wm2[3][2] };
								ImGui::DragFloat3("Position", &pos2[0], 0.01f, -100.0f, 100.0f);
								wm2[3][0] = pos2.x;
								wm2[3][1] = pos2.y;
								wm2[3][2] = pos2.z;
								std::vector<Mesh*>& meshes = models[j].getMeshes();
								for (int k = 0; k < meshes.size(); k++)
								{
									std::string meshName("Mesh " + std::to_string(k));
									if (ImGui::TreeNode(meshName.c_str()))
									{
										ImGui::Text("This is a mesh!");
										ImGui::TreePop();
									}
								}
								ImGui::TreePop();
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::End();
			}

			// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow().
			if (show_test_window)
			{
				ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
				ImGui::ShowTestWindow(&show_test_window);
			}
		}
		// ------------------------------------------------------------------------


		// Compute deltat time (dt)
		currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float>(currentTime-previousTime).count();
		previousTime = currentTime;

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

	if (this->stateLoader.getEntities()->size() > 0)
	{
		Entity * temp = (*this->stateLoader.getEntities())[0];
		temp->getModel(0).setLocalMatrix(glm::rotate(temp->getModel(0).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f }));
		temp->getModel(1).setLocalMatrix(glm::rotate(temp->getModel(1).getLocalMatrix(), dt*3.0f, { 0.0f, 1.0f, 0.0f }));
		temp->setLocalMatrix(glm::rotate(temp->getLocalMatrix(), -dt, { 0.0f, 1.0f, 0.0f }));
	}
	
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

	// Draw ImGui elements.
	ImGui::Render();
	// Swap buffers
	glfwSwapBuffers(display.getWindowPtr());
}

