#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <map>
#include <glm.hpp>

#include "Models\Mesh.h"
#include "Node.h"
#include "../Shading/ShaderProgram.h"

class Component;
class Display;

class Entity : public Node
{
public:
	Entity(const glm::vec3 &position, const glm::vec3 &direction, bool isDynamic = false);
	Entity(const glm::mat4 &matrix = glm::mat4(1.0f), bool isDynamic = false);
	~Entity();

	void addMesh(Mesh* mesh, ShaderProgram* shader);
	void addComponent(Component* component);

	std::vector<Mesh*>& getMeshes();
private:
	std::vector<Component*> components;

	std::vector<Mesh*> meshes;
	std::vector<ShaderProgram*> shaders;
	std::map<ShaderProgram*, std::vector<unsigned int>> shaderMap;

	void selfInit();
	void selfUpdate(float dt);
	void selfRender();
	void selfInput(Display* display);

	bool isDynamic;
};

#endif
