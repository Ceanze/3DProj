#ifndef ENGINECORE_H
#define ENGINECORE_H

#include "States\StateManager.h"
#include "Display.h"
#include "Config.h"

/*---------------- TEMP --------------------*/
#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_glfw_gl3.h"

#include "../Entities/Models/Mesh.h"
#include "../Entities/Entity.h"
#include "../Loaders/ObjLoader.h"
#include "../Shading/Shader.h"

#include "../Shading/ShaderProgram.h"
#include "../Shading/Deferred Rendering/GeometryShader.h"

#include "Renderers\DeferredRenderer.h"
/*-------------- END TEMP ------------------*/

#include "../Entities/Components/Camera/Camera.h"
#include "Frustum Culling\Frustum.h"


class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	void init();

private:
	void update(const float& dt);
	void render();
	void input(Display* display);

	void renderGui();
	void renderNodeGUI(Node* e, int level = 0);
	void renderDRTextures();
	void renderLSTextures();
	void renderBrightnessTextures();
	void renderBlurTextures();
	void renderTexture(ImTextureID texID, float ratio, bool nextLine);

	void attachCamera(Camera* camera);
	void swapCamera();

	Display display;

	/*---------------- TEMP --------------------*/
	GeometryShader* geometryShader;
	DeferredRenderer* deferredRenderer;

	Terrain terrain;
	Entity* base;
	Entity* e1;
	Entity* e2;
	Entity* armyPilot;
	Entity* sword;
	Entity* lightBase;
	Entity* directionalLight;
	std::vector<Entity*> arm;
	Mesh* m1;
	Mesh* m2;
	std::vector<Mesh*> cubeMeshes;
	std::vector<Mesh*> swordMeshes;
	std::vector<Mesh*> armyPilotMeshes;
	ObjLoader loader;
	Camera* camera;
	Camera* camera2;
	Camera* shadowCamera;
	Camera* activeCamera;
	Frustum* frustum;

	unsigned int camInc;
	/*-------------- END TEMP ------------------*/
};

#endif // !ENGINECORE_H
