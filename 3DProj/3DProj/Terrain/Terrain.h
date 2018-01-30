#ifndef TERRAIN_H
#define TERRAIN_H

#include "glm.hpp"
#include "glew.h"

#include "../Shading/ShaderProgram.h"
#include "../Core/GL Utils/Texture.h"
#include "../Terrain/QuadTree.h"

#include <vector>

#define MAX_PIXEL_COLOR 256
#define MAX_HEIGHT 10

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uvs;
};

class Terrain
{
public:
	Terrain();
	~Terrain();

	void render();
	void setShader(ShaderProgram* shader);

private:
	void loadTexture(const std::string& path, Texture** texture);
	void generateTerrain();
	void generateVerticies();
	void generateIndicies(const unsigned& x, const unsigned& z);
	const glm::vec3& generateNormals(const unsigned& x, const unsigned& z, unsigned char* data);

	float getHeight(const unsigned& x, const unsigned& z, unsigned char* data);

	void loadToGPU();

	GLuint addVao();
	GLuint addVertexVbo();
	GLuint addEbo();

	bool flatShading;

	GLuint vao, vertexVbo, normalVbo, ebo;
	GLuint vPosLocation, normalLocation, uvsLocation;

	unsigned size, offset, rowLength;

	std::vector<glm::vec3> normals;
	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;

	QuadTree quadTree;

	ShaderProgram* shader;
	Texture* heightMap;
	Texture* texture;
	GLuint textureLocation;
};

#endif
