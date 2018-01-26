#include "Terrain.h"

#include "../Error.h"
#include "../Core/ResourceManager.h"

Terrain::Terrain(const unsigned & size, const unsigned& offset)
{
	this->flatShading = false;
	this->size = size;
	this->offset = offset;

	this->heightMap = nullptr;
	this->loadTexture("./Resources/Textures/heightmaptest.png", &this->heightMap);

	this->texture = nullptr;
	this->loadTexture("./Resources/Textures/stone.jpg", &this->texture);
	this->texture->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	this->texture->unbind();

	if (this->flatShading)
		this->generateTerrainFlat();
	else
		this->generateTerrain();
}

Terrain::~Terrain()
{
	delete this->texture;
	delete this->heightMap;
	glDeleteBuffers(1, &this->vertexVbo);
	glDeleteBuffers(1, &this->normalVbo);
	glDeleteVertexArrays(1, &this->vao);
}

void Terrain::render()
{
	glUseProgram(this->shader->getID());

	glUniform1i(this->textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getTexture());

	glBindVertexArray(this->vao );
	this->shader->updateUniforms();

	if (this->flatShading)
		glDrawArrays(GL_TRIANGLES, 0, this->verticies.size());
	else
		glDrawElements(GL_TRIANGLES, this->indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void Terrain::setShader(ShaderProgram * shader)
{
	this->shader = shader;

	this->textureLocation = glGetUniformLocation(this->shader->getID(), "albedoMap");

	this->loadToGPU();
}

void Terrain::loadTexture(const std::string& path, Texture** texture)
{
	char hasFailed = ResourceManager::loadTexture(path, texture);
	if (hasFailed == TEXTURE_FAILED)
		Error::printError("Failed to load texture: " + path);
	else if (hasFailed == TEXTURE_SUCCEEDED)
		Error::printWarning("Loaded texture:  " + path);
}

void Terrain::generateTerrain()
{
	this->generateVerticies();
	this->generateNormals();

}

void Terrain::generateTerrainFlat()
{
	glm::vec3 start(-(int)this->size / 2, 0, -(int)this->size / 2);

	const unsigned rowLength = this->size / this->offset;

	for (unsigned x = 0; x <= rowLength; x++)
	{
		for (unsigned z = 0; z <= rowLength; z++)
		{
			this->generateQuad(x, z, rowLength, start);
		}
	}
}

void Terrain::generateVerticies()
{
	glm::vec3 start(-(int)this->size / 2, 0, -(int)this->size / 2);

	const unsigned rowLength = this->size / this->offset;

	Vertex vertex;
	unsigned char hMapPixel;

	//Get HeightMap pixel color values
	const std::vector<TextureInfo>& textureData = this->heightMap->getTextureData();
	TextureInfo s = textureData[0];
	unsigned char* data = (unsigned char*)s.data;

	for (unsigned x = 0; x <= rowLength; x++)
	{ 
		for (unsigned z = 0; z <= rowLength; z++)
		{
			hMapPixel = data[(x * rowLength + z) * 4];
			
			vertex.position = start + glm::vec3(offset * x, this->getHeight(hMapPixel), offset * z);
			vertex.uvs = glm::vec2(x, z);
			this->verticies.push_back(vertex);

			if (x >= 1 & z >= 1)
			{
				this->generateIndicies(x - 1, z - 1, rowLength);
			}
		}
	}

	
}

void Terrain::generateIndicies(const unsigned& x, const unsigned& z, const unsigned& rowLength)
{
	//triangle 1
	this->indicies.push_back(z + x * (rowLength + 1));
	this->indicies.push_back(z + 1 + x * (rowLength + 1));
	this->indicies.push_back(z + x * (rowLength + 1) + (rowLength + 1));

	this->generateNormals();

	//triangle 2
	this->indicies.push_back(z + 1 + x * (rowLength + 1) + (rowLength + 1));
	this->indicies.push_back(z + x * (rowLength + 1) + (rowLength + 1));
	this->indicies.push_back(z + 1 + x * (rowLength + 1));

	this->generateNormals();
}

void Terrain::generateNormals()
{
	/*glm::vec3 v1 = this->verticies[this->indicies[this->indicies.size() - 2]].position - this->verticies[this->indicies[this->indicies.size() - 3]].position;
	glm::vec3 v2 = this->verticies[this->indicies[this->indicies.size() - 1]].position - this->verticies[this->indicies[this->indicies.size() - 3]].position;

	glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

	for (unsigned i = 0; i < 3; i++)
	{
		this->normals.push_back(normal);
	}*/

	for (int i = 0; i < this->verticies.size(); i++) {
		this->normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

float Terrain::getHeight(const unsigned char & r)
{
	float height = ((float)r / MAX_PIXEL_COLOR) * MAX_HEIGHT;

	return height;
}

void Terrain::generateQuad(const unsigned & x, const unsigned & z, const unsigned & rowLength, const glm::vec3& start)
{
	this->setPosAndUvs(start, offset * x, sin(x+z) * 10, offset * z, glm::vec2(x, z));
	this->setPosAndUvs(start, offset * x, sin(x + z) * 10, offset * z + offset, glm::vec2(x, z + 1));
	this->setPosAndUvs(start, offset * x + offset, sin(x + z) * 10, offset * z, glm::vec2(x + 1, z));

	this->setPosAndUvs(start, offset * x, sin(x + z) * 10, offset * z + offset, glm::vec2(x, z + 1));
	this->setPosAndUvs(start, offset * x + offset, sin(x + z) * 10, offset * z + offset, glm::vec2(x + 1, z + 1));
	this->setPosAndUvs(start, offset * x + offset, sin(x + z) * 10, offset * z, glm::vec2(x + 1, z));
}

void Terrain::setPosAndUvs(const glm::vec3& start, const unsigned & x, const unsigned & y, const unsigned & z, const glm::vec2& uvs)
{
	Vertex vertex;
	vertex.position = start + glm::vec3(x, y, z);
	vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
	vertex.uvs = uvs;
	this->verticies.push_back(vertex);
}



void Terrain::loadToGPU()
{
	this->vao = addVao();
	this->vertexVbo = addVertexVbo();
	if (!this->flatShading)
	{ 
		this->normalVbo = addNormalsVbo();
		this->ebo = addEbo();
	}
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

	this->vPosLocation = glGetAttribLocation(this->shader->getID(), "vertexPosition");
	if (this->vPosLocation == -1)
		Error::printError("Terrain couldn't find 'vertexPosition' in GeometryDR.vs");

	glEnableVertexAttribArray(vPosLocation);
	glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr);

	if (this->flatShading)
	{
		this->normalLocation = glGetAttribLocation(this->shader->getID(), "vertexNormal");
		if (this->normalLocation == -1)
			Error::printError("Terrain couldn't find 'vertexNormal' in GeometryDR.vs");
		glEnableVertexAttribArray(this->normalLocation);
	}
	glEnableVertexAttribArray(this->normalLocation);
	glVertexAttribPointer(this->normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 3));


	this->uvsLocation = glGetAttribLocation(this->shader->getID(), "vertexUvs");
	if (this->uvsLocation == -1)
		Error::printError("Terrain couldn't find 'vertexUvs' in GeometryDR.vs");
	glEnableVertexAttribArray(uvsLocation);
	glVertexAttribPointer(uvsLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 6));



	
	glBindVertexArray(0);

	return vbo;
}

GLuint Terrain::addNormalsVbo()
{
	GLuint vbo;

	glBindVertexArray(this->vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);

	this->normalLocation = glGetAttribLocation(this->shader->getID(), "vertexNormal");
	if (this->normalLocation == -1)
		Error::printError("Terrain couldn't find 'vertexNormal' in GeometryDR.vs");

	glEnableVertexAttribArray(normalLocation);

	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (char*)(sizeof(GLfloat) * 0));

	return vbo;
}

GLuint Terrain::addEbo()
{
	glBindVertexArray(this->vao);

	GLuint ebo;

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->indicies.size(), &this->indicies[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	return ebo;
}
