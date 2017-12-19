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

	Display display;

	StateManager states;
	StateLoader stateLoader;

	/*---------------- TEMP --------------------*/
	ObjLoader loader;
	Mesh* mesh = nullptr;
	Mesh* mesh2 = nullptr;
	Shader* vsShader = nullptr;
	Shader* fsShader = nullptr;
	ShaderProgram* sp = nullptr;
	Model* model = nullptr;
	Model* model2 = nullptr;
	Model* model3 = nullptr;
	Entity* entity = nullptr;
	/*-------------- END TEMP ------------------*/
};

#endif // !ENGINECORE_H
