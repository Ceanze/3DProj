#ifndef MESH_H
#define MESH_H

#include <glew.h>
#include <glm.hpp>
#include <vector>

#include "Material.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uvs;
	};

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Material* material = nullptr;
};

#endif
