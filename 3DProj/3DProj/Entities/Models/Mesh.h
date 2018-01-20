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

	void loadToGPU(GLuint shaderProgramID, GLenum usage = GL_STATIC_DRAW, bool useUvs = false);

	void draw() const;

	GLuint getVAO() const;

public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec2 uvs;
	};

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	Material* material = nullptr;

private:
	GLuint vao;
	GLuint vbo;
	GLuint indexBufferID;

	GLint vertexPosID;
	GLint vertexNormalID;
	GLint vertexTangentID;
	GLint vertexUvsID;

	GLint textureLocation;
	GLint normalMapLocation;
};

#endif
