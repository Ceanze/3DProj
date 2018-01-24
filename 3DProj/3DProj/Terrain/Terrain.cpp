#include "Terrain.h"

#include "../Error.h"

Terrain::Terrain(const unsigned & size, const unsigned& offset)
{
	this->size = size;
	this->offset = offset;

	this->generateTerrain();
}

Terrain::~Terrain()
{
	glDeleteBuffers(1, &this->vertexVbo);
	glDeleteBuffers(1, &this->normalVbo);
	glDeleteVertexArrays(1, &this->vao);
}

void Terrain::render()
{
	glUseProgram(this->shader->getID());
	glBindVertexArray( this->vao );
	this->shader->updateUniforms();

	glDrawElements(GL_TRIANGLES, this->indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
}

void Terrain::setShader(ShaderProgram * shader)
{
	this->shader = shader;
	this->loadToGPU();
}

void Terrain::generateTerrain()
{
	this->generateVerticies();
	this->generateNormals();

}

void Terrain::generateVerticies()
{
	glm::vec3 start(-(int)this->size / 2, 0, -(int)this->size / 2);

	const unsigned rowLength = this->size / this->offset;

	Vertex vertex;

	for (unsigned x = 0; x <= rowLength; x++)
	{
		for (unsigned z = 0; z <= rowLength; z++)
		{
			vertex.position = start + glm::vec3(offset * x, 0, offset * z);
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
	glm::vec3 v1 = this->verticies[this->indicies[this->indicies.size() - 2]].position - this->verticies[this->indicies[this->indicies.size() - 3]].position;
	glm::vec3 v2 = this->verticies[this->indicies[this->indicies.size() - 1]].position - this->verticies[this->indicies[this->indicies.size() - 3]].position;

	glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

	for (unsigned i = 0; i < 3; i++)
	{
		this->normals.push_back(normal);
	}
}

void Terrain::loadToGPU()
{
	this->vao = addVao();
	this->vertexVbo = addVertexVbo();
	this->normalVbo = addNormalsVbo();
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

	this->vPosLocation = glGetAttribLocation(this->shader->getID(), "vertexPosition");
	if (this->vPosLocation == -1)
		Error::printError("Terrain couldn't find 'vertexPosition' in GeometryDR.vs");

	this->uvsLocation = glGetAttribLocation(this->shader->getID(), "vertexUvs");
	if (this->uvsLocation == -1)
		Error::printError("Terrain couldn't find 'vertexUvs' in GeometryDR.vs");

	glEnableVertexAttribArray(vPosLocation);
	glEnableVertexAttribArray(uvsLocation);

	glVertexAttribPointer(vPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr);
	glVertexAttribPointer(uvsLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 3));
	
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
