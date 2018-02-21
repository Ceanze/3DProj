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
	glm::vec3 tangent;
	glm::vec2 uvs;
};

class Terrain
{
public:
	Terrain(const unsigned& terrainScale = 2, const float& textureScale = 8);
	~Terrain();

	void render(ShaderProgram* shadowShader = nullptr);
	void setShader(ShaderProgram* shader);

	float getHeight(const float& x, const float& z);

	QuadTree*  getQuadTree();

private:
	void loadTexture(const std::string& path, Texture** texture, const bool& generateMIPMAP = true);
	void generateTerrain();
	void generateVerticies();
	void generateIndicies(const unsigned& x, const unsigned& z);
	const glm::vec3 generateNormals(const unsigned& x, const unsigned& z, unsigned char* data);
	void generateTangent(const Triangle& tri);
	glm::vec3 getTriangleMidPoint(const Triangle& tri) const;

	float getHeight(const unsigned& x, const unsigned& z, unsigned char* data);

	void loadToGPU();

	GLuint addVao();
	GLuint addVertexVbo();
	GLuint addEbo();

	GLuint vao, vertexVbo, ebo;
	GLuint vPosLocation, normalLocation, uvsLocation, tangentLocation;
	GLint useNormalMapLoc;

	float* heights;
	float textureScale;
	unsigned size, offset, rowLength;
	glm::vec3 start;

	std::vector<Vertex> verticies;
	std::vector<GLuint> indicies;

	QuadTree* quadTree;

	ShaderProgram* shader;
	Texture* heightMap;
	Texture *texture, *textureNormalMap;
	GLuint textureLocation, textureNPlocation, hasTextureLocation;

};

#endif
