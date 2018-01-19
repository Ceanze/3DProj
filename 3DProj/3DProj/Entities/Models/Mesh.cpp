#include "Mesh.h"

#include <cassert>

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	delete this->material;
	glDeleteBuffers(1, &(this->vbo));
	glDeleteBuffers(1, &(this->indexBufferID));
	glDeleteVertexArrays(1, &(this->vao));

	glDisableVertexAttribArray(this->vertexPosID);
	glDisableVertexAttribArray(this->vertexNormalID);
	glDisableVertexAttribArray(this->vertexUvsID);
}

void Mesh::loadToGPU(GLuint shaderProgramID, GLenum usage, bool useUvs)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*this->vertices.size(), &this->vertices[0], usage);

	glUseProgram(shaderProgramID);

	// Position
	this->vertexPosID = glGetAttribLocation(shaderProgramID, "vertexPosition");
	assert(this->vertexPosID != -1 && "Error, cannot find 'vertexPosition' attribute in Vertex shader\n");
	glEnableVertexAttribArray(this->vertexPosID); // This is now affecting the vaoID which was created. 
	glVertexAttribPointer(this->vertexPosID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr);

	// Noraml
	this->vertexNormalID = glGetAttribLocation(shaderProgramID, "vertexNormal");
	assert(this->vertexNormalID != -1 && "Error, cannot find 'vertexNormal' attribute in Vertex shader\n");
	glEnableVertexAttribArray(this->vertexNormalID);
	glVertexAttribPointer(this->vertexNormalID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 3));

	// Texture coords
	this->vertexUvsID = glGetAttribLocation(shaderProgramID, "vertexUvs");
	assert(this->vertexUvsID != -1 && "Error, cannot find 'vertexUvs' attribute in Vertex shader\n");
	glEnableVertexAttribArray(this->vertexUvsID);
	glVertexAttribPointer(this->vertexUvsID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 6));

	// Indices
	glGenBuffers(1, &(this->indexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->indices.size(), &this->indices[0], usage);

	glBindVertexArray(0);
}

void Mesh::draw() const
{
	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint Mesh::getVAO() const
{
	return this->vao;
}
