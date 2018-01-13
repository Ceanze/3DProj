#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <map>
#include <glm.hpp>

#include "Models\Model.h"

#include "Node.h"

#include "../Shading/ShaderProgram.h"

class Entity : public Node
{
public:
	Entity(const glm::vec3 &position, const glm::vec3 &direction, bool isDynamic = false);
	Entity(const glm::mat4 &matrix = glm::mat4(1.0f), bool isDynamic = false);
	~Entity();

	void addMesh(Mesh* mesh, ShaderProgram* shader);
private:
	std::vector<Mesh*> meshes;
	std::vector<ShaderProgram*> shaders;
	std::map<ShaderProgram*, std::vector<unsigned int>> shaderMap;

	void selfUpdate(float dt);
	void selfRender();

	bool isDynamic;
};

#endif
