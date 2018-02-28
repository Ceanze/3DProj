#include "Terrain.h"

#include "../Utils/Utils.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../Error.h"
#include "../Core/ResourceManager.h"
#include "../Loaders/MTLLoader.h"

Terrain::Terrain(const unsigned& terrainScale, const float& textureScale)
	: quadTree(nullptr)
{
	this->mesh = new Mesh();
	this->mesh->material = MTLLoader::load("../Terrain/terrain.mtl", true)["TerrainMtl"];
	this->textureScale = textureScale;

	this->mesh->material->texture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->mesh->material->normalMap->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	this->mesh->material->normalMap->unbind();

	this->heightMap = nullptr;

	this->loadTexture("./Resources/Terrain/heightmap.png", &this->heightMap, false);
	
	this->size = this->heightMap->getWidth() * terrainScale;
	this->offset = terrainScale;
	this->rowLength = (this->size / this->offset);
	this->start = glm::vec3(-(int)this->size / 2, 0, -(int)this->size / 2);

	this->heights = new float[(int)size * (int)size];

	glm::vec3 topLeftCorner(this->start.x, 0, this->start.z);
	glm::vec3 topRightCorner(this->start.x + this->size, 0, this->start.z);
	glm::vec3 botLeftCorner(this->start.x, 0, this->start.z + this->size);
	glm::vec3 botRightCorner(this->start.x + this->size, 0, this->start.z + this->size);

	glm::vec3 corners[4] = { topLeftCorner, topRightCorner, botLeftCorner, botRightCorner };

	quadTree = new QuadTree(4, corners, 10);

	this->generateTerrain();

	
}

Terrain::~Terrain()
{
	delete[] heights;

	delete this->mesh->material;
	delete this->mesh;
	delete this->quadTree;
	delete this->heightMap;
}

void Terrain::render(ShaderProgram* shadowShader)
{
	if(shadowShader == nullptr)
		glUseProgram(this->shader->getID());

	//glUniform1i(this->useNormalMapLoc, 1);

	//Has texture, should be 1 if texture is attached to textureMap
	//glUniform1i(this->hasTextureLocation, 1);

	this->mesh->prepareForDraw();

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

	this->mesh->loadToGPU(this->shader->getID(), GL_STATIC_DRAW, false);

	glBindVertexArray(this->mesh->getVAO());
	this->quadTree->addEbo();
	glBindVertexArray(0);
}

float Terrain::getHeight(const float & x, const float & z)
{
	float height = 0.0f;

	float terrainX = -1 * (this->start.x - x);
	float terrainZ = -1 * (this->start.z - z);
	
	glm::u8vec2 gridPos(floorf(terrainX / this->offset), floorf(terrainZ / this->offset) - 2);

	if (gridPos.x >= this->rowLength - 1 || gridPos.y >= this->rowLength - 1 || gridPos.x < 0 || gridPos.y < 0)
	{
		return 0.0f;
	}
	
	float xCoord = (float)fmod(terrainX, this->offset) / this->offset;
	float zCoord = (float)fmod(terrainZ, this->offset) / this->offset;
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

QuadTree * Terrain::getQuadTree()
{
	return this->quadTree;
}

void Terrain::loadTexture(const std::string& path, Texture** texture, const bool& generateMIPMAP)
{
	char hasFailed = ResourceManager::loadTexture(path, texture);
	if (hasFailed == TEXTURE_FAILED)
		Error::printError("Failed to load texture: " + path);
	else if (hasFailed == TEXTURE_SUCCEEDED)
		Error::print("[Loaded texture]:", path);

	if (generateMIPMAP)
	{
		texture[0][0].bind();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		texture[0][0].unbind();
	}
}

void Terrain::generateTerrain()
{
	this->generateVerticies();
	this->quadTree->init();
}

void Terrain::generateTangent(const Triangle& tri)
{

	Mesh::Vertex& v0 = this->mesh->vertices[tri.p1];
	Mesh::Vertex& v1 = this->mesh->vertices[tri.p2];
	Mesh::Vertex& v2 = this->mesh->vertices[tri.p3];

	glm::vec2 deltaUV1 = v1.uvs - v0.uvs;
	glm::vec2 deltaUV2 = v2.uvs - v0.uvs;
	glm::vec3 deltaPos1 = v1.position - v0.position;
	glm::vec3 deltaPos2 = v2.position - v0.position;

	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;

	// Make the tangent perpendicular to its corresponding normal.
	v0.tangent = glm::normalize(tangent - v0.normal*glm::dot(v0.normal, tangent));
	v1.tangent = glm::normalize(tangent - v1.normal*glm::dot(v1.normal, tangent));
	v2.tangent = glm::normalize(tangent - v2.normal*glm::dot(v2.normal, tangent));

}

glm::vec3 Terrain::getTriangleMidPoint(const Triangle & tri) const
{
	glm::vec3 midPoint = (this->mesh->vertices[tri.p1].position + this->mesh->vertices[tri.p2].position + this->mesh->vertices[tri.p3].position) / 3.0f;

	return midPoint;
}

void Terrain::generateVerticies()
{
	Mesh::Vertex vertex;

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
			vertex.uvs = glm::vec2(x/ this->textureScale, z/ this->textureScale);
			this->mesh->vertices.push_back(vertex);

			if (x >= 1 && z >= 1)
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

	glm::vec3 triMidPoint = this->getTriangleMidPoint(tri1);
	glm::vec2 pos(triMidPoint.x, triMidPoint.z);
	this->quadTree->addTriangleToRoot(pos, tri1);

	triMidPoint = this->getTriangleMidPoint(tri2);
	pos = { triMidPoint.x, triMidPoint.z };
	this->quadTree->addTriangleToRoot(pos, tri2);


	this->generateTangent(tri1);
	this->generateTangent(tri2);

}

const glm::vec3 Terrain::generateNormals(const unsigned& x, const unsigned& z, unsigned char* data)
{
	float LH, RH, UH, DH;

	LH = this->getHeight(x, z - 1, data);
	RH = this->getHeight(x, z + 1, data);
	UH = this->getHeight(x - 1, z, data);
	DH = this->getHeight(x + 1, z, data);

	glm::vec3 normal = glm::normalize(glm::vec3(LH - RH, 2.f, DH - UH));
	glm::mat4 rotateMatrix(1.0f);
	rotateMatrix = glm::rotate(rotateMatrix, -Tools::PI / 2.0f, glm::vec3(0.0, 1.0, 0.0));

	glm::vec4 rotatedNormal = (rotateMatrix * glm::vec4(normal, 1.0f));

	normal.x = rotatedNormal.x;
	normal.y = rotatedNormal.y;
	normal.z = rotatedNormal.z;

	return normal;
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
	//this->vao = addVao();
	//this->vertexVbo = addVertexVbo();
	//this->ebo = addEbo();
}

GLuint Terrain::addVao()
{
	GLuint vao;
	//glGenVertexArrays(1, &vao);
	//
	return vao;
}

GLuint Terrain::addVertexVbo()
{
	GLuint vbo;
	//glBindVertexArray(this->vao);
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, this->mesh->vertices.size() * sizeof(Mesh::Vertex), &this->mesh->vertices[0], GL_STATIC_DRAW);

	//this->vPosLocation = 0;// glGetAttribLocation(this->shader->getID(), "vertexPosition");
	//if (this->vPosLocation == -1)
	//	Error::printError("Terrain couldn't find 'vertexPosition' in GeometryDR.vs");

	//glEnableVertexAttribArray(vPosLocation);
	//glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::Vertex), (char*)nullptr);

	//this->normalLocation = 1;// glGetAttribLocation(this->shader->getID(), "vertexNormal");
	//if (this->normalLocation == -1)
	//	Error::printError("Terrain couldn't find 'vertexNormal' in GeometryDR.vs");
	//glEnableVertexAttribArray(this->normalLocation);
	//glVertexAttribPointer(this->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 3));

	//this->tangentLocation = 2;// glGetAttribLocation(this->shader->getID(), "vertexUvs");
	//if (this->tangentLocation == -1)
	//	Error::printError("Terrain couldn't find 'vertexTangent' in GeometryDR.vs");
	//glEnableVertexAttribArray(tangentLocation);
	//glVertexAttribPointer(tangentLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 6));

	//this->uvsLocation = 3;// glGetAttribLocation(this->shader->getID(), "vertexUvs");
	//if (this->uvsLocation == -1)
	//	Error::printError("Terrain couldn't find 'vertexUvs' in GeometryDR.vs");
	//glEnableVertexAttribArray(uvsLocation);
	//glVertexAttribPointer(uvsLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 9));

	//
	//this->useNormalMapLoc = glGetUniformLocation(this->shader->getID(), "useNormalMap");
	//if (this->useNormalMapLoc == -1)
	//	Error::printError("Could not find 'useNormalMap' in shader!");

	//this->hasTextureLocation = glGetUniformLocation(this->shader->getID(), "hasTexture");
	//if (this->hasTextureLocation == -1)
	//	Error::printError("Could not find 'hasTexture' in shader");

	//glBindVertexArray(0);

	return vbo;
}

GLuint Terrain::addEbo()
{
	/*glBindVertexArray(this->vao);

	this->quadTree->addEbo();
*/
	return 0;
}
