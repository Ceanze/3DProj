#include "Terrain.h"

#include "../Utils/Utils.h"
#include "../glm/glm.hpp"
#include "../Error.h"
#include "../Core/ResourceManager.h"
#include <iostream>

Terrain::Terrain()
	: quadTree(nullptr)
{
	this->heightMap = nullptr;
	this->loadTexture("./Resources/Textures/heightmapTest.png", &this->heightMap);

	this->texture = nullptr;
	this->loadTexture("./Resources/Textures/stone.jpg", &this->texture);
	this->texture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	this->size = this->heightMap->getWidth() * 2;
	this->offset = 2;
	this->rowLength = this->size / this->offset;
	this->start = glm::vec3(-(int)this->size / 2, 0, -(int)this->size / 2);
	this->texture->unbind();

	this->heights = new float[(int)size * (int)size];

	glm::vec2 topLeftCorner(this->start.x, this->start.z);
	glm::vec2 topRightCorner(this->start.x + this->size, this->start.z);
	glm::vec2 botLeftCorner(this->start.x, this->start.z + this->size);
	glm::vec2 botRightCorner(this->start.x + this->size, this->start.z + this->size);

	glm::vec2 corners[4] = { topLeftCorner, topRightCorner, botLeftCorner, botRightCorner };

	quadTree = new QuadTree(1, corners);

	this->generateTerrain();
}

Terrain::~Terrain()
{
	delete[] heights;

	delete this->quadTree;
	delete this->texture;
	delete this->heightMap;
	glDeleteBuffers(1, &this->vertexVbo);
	glDeleteVertexArrays(1, &this->vao);
}

void Terrain::render(ShaderProgram* shadowShader)
{
	if(shadowShader == nullptr)
		glUseProgram(this->shader->getID());

	glUniform1i(this->useNormalMapLoc, 0);

	glUniform1i(this->textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTexture());

	glBindVertexArray(this->vao );
	if (shadowShader == nullptr)
		this->shader->updateUniforms();
	else 
		shadowShader->updateUniforms();

	this->quadTree->render();

	glBindVertexArray(0);
	if(shadowShader == nullptr)
		glUseProgram(0);
}

void Terrain::setShader(ShaderProgram * shader)
{
	this->shader = shader;

	this->textureLocation = glGetUniformLocation(this->shader->getID(), "albedoMap");

	this->loadToGPU();
}

float Terrain::getHeight(const float & x, const float & z)
{
	float height = 0.0f;

	float terrainX = -1 * (this->start.x - x);
	float terrainZ = -1 * (this->start.z - z);
	
	glm::u8vec2 gridPos(floorf(terrainX / this->offset), floorf(terrainZ / this->offset));

	if (gridPos.x >= this->rowLength || gridPos.y >= this->rowLength || gridPos.x < 0 || gridPos.y < 0)
	{
		return 0.0f;
	}

	float xCoord = fmod(terrainX, this->offset) / this->offset;
	float zCoord = fmod(terrainZ, this->offset) / this->offset;
	float a, b, c;

	if (xCoord <= (1 - zCoord))
	{
		a = this->heights[gridPos.x + gridPos.y * this->rowLength];
		b = this->heights[gridPos.x + 1 + gridPos.y * this->rowLength];
		c = this->heights[gridPos.x + (gridPos.y + 1) * this->rowLength];

		height = Tools::barryCentricHeight(glm::vec3(0, a, 0)
			, glm::vec3(1, b, 0)
			, glm::vec3(0, c, 1)
			, glm::vec2(xCoord, zCoord));
	}
	else
	{
		a = this->heights[gridPos.x + 1 + (gridPos.y + 1) * this->rowLength];
		b = this->heights[gridPos.x + 1 + gridPos.y * this->rowLength];
		c = this->heights[gridPos.x + (gridPos.y + 1) * this->rowLength];

		height = Tools::barryCentricHeight(glm::vec3(1, b, 0)
			, glm::vec3(1, a, 1)
			, glm::vec3(0, c, 1)
			, glm::vec2(xCoord, zCoord));
	}

	return height;
}

void Terrain::loadTexture(const std::string& path, Texture** texture)
{
	char hasFailed = ResourceManager::loadTexture(path, texture);
	if (hasFailed == TEXTURE_FAILED)
		Error::printError("Failed to load texture: " + path);
	else if (hasFailed == TEXTURE_SUCCEEDED)
		Error::print("[Loaded texture]:", path);
}

void Terrain::generateTerrain()
{
	this->generateVerticies();
}

void Terrain::generateVerticies()
{
	Vertex vertex;
	unsigned char hMapPixel;

	//Get HeightMap pixel color values
	const std::vector<TextureInfo>& textureData = this->heightMap->getTextureData();
	TextureInfo s = textureData[0];
	unsigned char* data = (unsigned char*)s.data;

	for (unsigned x = 0; x < rowLength; x++)
	{ 
		for (unsigned z = 0; z < rowLength; z++)
		{
			this->heights[x + z * rowLength] = this->getHeight(x, z, data);
			vertex.position = this->start + glm::vec3(offset * x, this->heights[x + z * rowLength], offset * z);
			vertex.normal = this->generateNormals(x, z, data);
			vertex.uvs = glm::vec2(x, z);
			this->verticies.push_back(vertex);

			if (x >= 1 & z >= 1)
			{
				this->generateIndicies(x - 1, z - 1);
			}
		}
	}
}

void Terrain::generateIndicies(const unsigned& x, const unsigned& z)
{
	Triangle tri1, tri2;
	tri1.p1 = z + x * (this->rowLength);
	tri1.p2 = z + 1 + x * (this->rowLength);
	tri1.p3 = z + x * (this->rowLength) + (this->rowLength);

	tri2.p1 = z + 1 + x * (this->rowLength) + (this->rowLength);
	tri2.p2 = z + x * (this->rowLength) + (this->rowLength);
	tri2.p3 = z + 1 + x * (this->rowLength);

	this->quadTree->addTriangle(glm::vec2(x * this->offset, z * this->offset), tri1);
	this->quadTree->addTriangle(glm::vec2(x * this->offset, z * this->offset), tri2);

}

const glm::vec3& Terrain::generateNormals(const unsigned& x, const unsigned& z, unsigned char* data)
{
	float LH, RH, UH, DH;

	LH = this->getHeight(x, z + 1, data);
	RH = this->getHeight(x, z - 1, data);
	UH = this->getHeight(x + 1, z, data);
	DH = this->getHeight(x - 1, z, data);

	return glm::normalize(glm::vec3(LH - RH, 2.f, DH - UH));
}

float Terrain::getHeight(const unsigned& x, const unsigned& z, unsigned char* data)
{
	if ((x < 0 || x > rowLength) || (z < 0 || z > rowLength))
		return 0;

	float height = data[(x * this->rowLength + z) * 4];

	height = ((float)height / MAX_PIXEL_COLOR) * MAX_HEIGHT;

	return height;
}

void Terrain::loadToGPU()
{
	this->vao = addVao();
	this->vertexVbo = addVertexVbo();
	this->ebo = addEbo();
}

GLuint Terrain::addVao()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	
	return vao;
}

GLuint Terrain::addVertexVbo()
{
	GLuint vbo;
	glBindVertexArray(this->vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->verticies.size() * sizeof(Vertex), &this->verticies[0], GL_STATIC_DRAW);

	this->vPosLocation = 0;// glGetAttribLocation(this->shader->getID(), "vertexPosition");
	if (this->vPosLocation == -1)
		Error::printError("Terrain couldn't find 'vertexPosition' in GeometryDR.vs");

	glEnableVertexAttribArray(vPosLocation);
	glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr);

	this->normalLocation = 1;// glGetAttribLocation(this->shader->getID(), "vertexNormal");
	if (this->normalLocation == -1)
		Error::printError("Terrain couldn't find 'vertexNormal' in GeometryDR.vs");
	glEnableVertexAttribArray(this->normalLocation);

	glVertexAttribPointer(this->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 3));

	this->uvsLocation = 3;// glGetAttribLocation(this->shader->getID(), "vertexUvs");
	if (this->uvsLocation == -1)
		Error::printError("Terrain couldn't find 'vertexUvs' in GeometryDR.vs");
	glEnableVertexAttribArray(uvsLocation);
	glVertexAttribPointer(uvsLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 6));
	
	this->useNormalMapLoc = glGetUniformLocation(this->shader->getID(), "useNormalMap");
	if (this->useNormalMapLoc == -1)
		Error::printError("Could not find 'useNormalMap' in shader!");

	glBindVertexArray(0);

	return vbo;
}

GLuint Terrain::addEbo()
{
	glBindVertexArray(this->vao);

	this->quadTree->addEbo();

	return 0;
}
