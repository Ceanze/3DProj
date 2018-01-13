#ifndef ENGINECORE_H
#define ENGINECORE_H

#include "..\Loaders\StateLoader.h"
#include "States\StateManager.h"
#include "Display.h"
#include "Config.h"

/*---------------- TEMP --------------------*/
#include "../Entities/Models/Mesh.h"
#include "../Entities/Models/Model.h"
#include "../Entities/Entity.h"
#include "../Loaders/ObjLoader.h"
#include "../Shading/Shader.h"

#include "../Shading/ShaderProgram.h"
#include "../Shading/PhongShader.h"
#include "../Shading/TestShader.h"
/*-------------- END TEMP ------------------*/

class EngineCore
{
public:
	EngineCore();
	~EngineCore();

	void init();

private:
	void update(const float& dt);
	void render();

	void renderGui();
	void renderNodeGUI(Node* e, int level = 0);

	Display display;

	//StateManager states;
	//StateLoader stateLoader;

	/*---------------- TEMP --------------------*/
	/*Shader* vsShader = nullptr;
	Shader* fsShader = nullptr;
	ShaderProgram* sp = nullptr;
	*/
	PhongShader* phongShader;
	TestShader* testShader;
	Entity* base;
	Entity* e1;
	Entity* e2;
	std::vector<Entity*> arm;
	Mesh* m1;
	Mesh* m2;
	ObjLoader loader;
	/*-------------- END TEMP ------------------*/
};

#endif // !ENGINECORE_H
