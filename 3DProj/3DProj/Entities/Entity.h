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

	void draw();

	void addMesh(Mesh* mesh, ShaderProgram* shader);
	//void addModel(const Model &model);
	//void updateMatrices(GLuint shaderProgramID, unsigned int modelIndex = 0);

	//Model& getModel(unsigned int index);
	//std::vector<Model>& getModels();
	/*
	glm::mat4 getTransformation(unsigned int modelIndex = 0) const; // Get this entitie's matrices combined with its model's matrix.
	glm::mat4 getEntityMatrix() const; // Get this class's matrices combined.
	glm::mat4& getLocalMatrix(); // Get this class's local matrix.
	glm::mat4& getWorldMatrix(); // Get this class's world matrix.

	void setLocalMatrix(const glm::mat4& mat);
	void setWorldMatrix(const glm::mat4& mat);
	*/
private:
	std::vector<Mesh*> meshes;
	std::vector<ShaderProgram*> shaders;
	std::map<ShaderProgram*, std::vector<unsigned int>> shaderMap;

	void selfUpdate(float dt);
	void selfRender();

	bool isDynamic;

	//glm::mat4 localMatrix;
	//glm::mat4 worldMatrix;
};

#endif
