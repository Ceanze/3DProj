#include "Mesh.h"

#include <cassert>
#include <string>

#include "../../Error.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &(this->vbo));
	glDeleteBuffers(1, &(this->indexBufferID));
	glDeleteVertexArrays(1, &(this->vao));

	glDisableVertexAttribArray(this->vertexPosID);
	glDisableVertexAttribArray(this->vertexNormalID);
	glDisableVertexAttribArray(this->vertexTangentID);
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

	if (this->material->normalMap != nullptr)
	{
		// Tangent
		this->vertexTangentID = glGetAttribLocation(shaderProgramID, "vertexTangent");
		assert(this->vertexTangentID != -1 && "Error, cannot find 'vertexTangent' attribute in Vertex shader\n");
		glEnableVertexAttribArray(this->vertexTangentID);
		glVertexAttribPointer(this->vertexTangentID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 6));
	}

	// Texture coords
	this->vertexUvsID = glGetAttribLocation(shaderProgramID, "vertexUvs");
	assert(this->vertexUvsID != -1 && "Error, cannot find 'vertexUvs' attribute in Vertex shader\n");
	glEnableVertexAttribArray(this->vertexUvsID);
	glVertexAttribPointer(this->vertexUvsID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 9));

	// Texture location
	this->textureLocation = glGetUniformLocation(shaderProgramID, "albedoMap");
	if (this->textureLocation == -1)
		Error::printError("Could not find 'albedoMap' in shader");

	// Normal map location
	if (this->material->normalMap != nullptr)
	{
		this->normalMapLocation = glGetUniformLocation(shaderProgramID, "normalMap");
		if (this->normalMapLocation == -1)
			Error::printError("Could not find 'normalMap' in shader");
	}

	initMaterialUniformBlock(shaderProgramID);

	// Indices
	glGenBuffers(1, &(this->indexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->indices.size(), &this->indices[0], usage);

	glBindVertexArray(0);
}

void Mesh::draw()
{
	loadMaterialToGPU();

	glUniform1i(this->textureLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->material->texture->getTexture());

	if (this->material->normalMap != nullptr)
	{
		glUniform1i(this->normalMapLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->material->normalMap->getTexture());
	}

	glBindVertexArray(this->vao);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLuint Mesh::getVAO() const
{
	return this->vao;
}

void Mesh::initMaterialUniformBlock(GLuint shaderProgramID)
{
	this->bindingPoint = 1;
	GLuint uboIndex = glGetUniformBlockIndex(shaderProgramID, "Material");
	glUniformBlockBinding(shaderProgramID, uboIndex, this->bindingPoint);

	glGenBuffers(1, &this->ubo);
}

void Mesh::loadMaterialToGPU()
{
	const float ambient = (this->material->ka.x+this->material->ka.y+this->material->ka.z)/3.0f;
	struct MaterialData
	{
		glm::vec4 kd_a;
		glm::vec4 ks_ns;
	} materialData = { glm::vec4(this->material->kd, ambient), glm::vec4(this->material->ks, this->material->ns)};
	
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), &materialData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, this->ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
