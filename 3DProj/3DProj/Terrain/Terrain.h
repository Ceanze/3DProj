#ifndef TERRAIN_H
#define TERRAIN_H

#include "glm.hpp"
#include "glew.h"

#include "../Shading/ShaderProgram.h"

#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uvs;
};

class Terrain
{
public:
	Terrain(const unsigned& size, const unsigned& offset);
	~Terrain();

	void render();
	void setShader(ShaderProgram* shader);

private:
	void generateTerrain();
	void generateVerticies();
	void generateIndicies(const unsigned& x, const unsigned& z, const unsigned& rowLength);
	void generateNormals();
	void loadToGPU();

	GLuint addVao();
	GLuint addVertexVbo();
	GLuint addNormalsVbo();
	GLuint addEbo();

	GLuint vao, vertexVbo, normalVbo, ebo;
	GLuint vPosLocation, normalLocation, uvsLocation;

	unsigned size, offset;

	std::vector<glm::vec3> normals;
	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;

	ShaderProgram* shader;
};

#endif