#ifndef TERRAIN_H
#define TERRAIN_H

#include "glm.hpp"
#include "glew.h"

#include "../Shading/ShaderProgram.h"
#include "../Core/GL Utils/Texture.h"

#include <vector>

#define MAX_PIXEL_COLOR 256
#define MAX_HEIGHT 2

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
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
	void loadTexture(const std::string& path, Texture** texture);
	void generateTerrain();
	void generateTerrainFlat();
	void generateVerticies();
	void generateIndicies(const unsigned& x, const unsigned& z, const unsigned& rowLength);
	void generateNormals();

	float getHeight(const unsigned char& r);

	void generateQuad(const unsigned& x, const unsigned& z, const unsigned& rowLength, const glm::vec3& start);
	void setPosAndUvs(const glm::vec3& start, const unsigned& x, const unsigned & y, const unsigned& z, const glm::vec2& uvs);

	void loadToGPU();

	GLuint addVao();
	GLuint addVertexVbo();
	GLuint addNormalsVbo();
	GLuint addEbo();

	bool flatShading;

	GLuint vao, vertexVbo, normalVbo, ebo;
	GLuint vPosLocation, normalLocation, uvsLocation;

	unsigned size, offset;

	std::vector<glm::vec3> normals;
	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;

	ShaderProgram* shader;
	Texture* heightMap;
	Texture* texture;
	GLuint textureLocation;
};

#endif
