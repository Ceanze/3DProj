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

void Mesh::loadToGPU(GLuint shaderProgramID, GLenum usage, bool useEBO)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*this->vertices.size(), &this->vertices[0], usage);

	glUseProgram(shaderProgramID);

	// Position
	this->vertexPosID = 0;
	glEnableVertexAttribArray(this->vertexPosID); // This is now affecting the vaoID which was created. 
	glVertexAttribPointer(this->vertexPosID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)nullptr);

	// Noraml
	this->vertexNormalID = 1;
	glEnableVertexAttribArray(this->vertexNormalID);
	glVertexAttribPointer(this->vertexNormalID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 3));

	if (this->material->normalMap != nullptr)
	{
		// Tangent
		this->vertexTangentID = 2;
		glEnableVertexAttribArray(this->vertexTangentID);
		glVertexAttribPointer(this->vertexTangentID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 6));
	}

	// Texture coords
	this->vertexUvsID = 3;
	glEnableVertexAttribArray(this->vertexUvsID);
	glVertexAttribPointer(this->vertexUvsID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*)(sizeof(GLfloat) * 9));

	this->useNormalMapLoc = glGetUniformLocation(shaderProgramID, "useNormalMap");
	if (this->useNormalMapLoc == -1)
		Error::printError("Could not find 'useNormalMap' in shader!");

	// Texture location
	this->textureLocation = glGetUniformLocation(shaderProgramID, "albedoMap");
	if (this->textureLocation == -1)
		Error::printError("Could not find 'albedoMap' in shader");

	// bool hasTexture location
	this->hasTextureLocation = glGetUniformLocation(shaderProgramID, "hasTexture");
	if (this->textureLocation == -1)
		Error::printError("Could not find 'hasTexture' in shader");

	// Normal map location
	if (this->material->normalMap != nullptr)
	{
		this->normalMapLocation = glGetUniformLocation(shaderProgramID, "normalMap");
		if (this->normalMapLocation == -1)
			Error::printError("Could not find 'normalMap' in shader");
	}

	initMaterialUniformBlock(shaderProgramID);

	if (useEBO)
	{
		// Indices
		glGenBuffers(1, &(this->indexBufferID));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*this->indices.size(), &this->indices[0], usage);

	}
	
	glBindVertexArray(0);
}

void Mesh::draw()
{
	this->prepareForDraw();
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::prepareForDraw()
{
	loadMaterialToGPU();
	glUniform1i(this->useNormalMapLoc, (int)(this->material->normalMap != nullptr));

	if (this->material->hasTexture)
	{
		glUniform1i(this->textureLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->material->texture->getTexture());
	}

	if (this->material->normalMap != nullptr)
	{
		glUniform1i(this->normalMapLocation, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->material->normalMap->getTexture());
	}

	glUniform1i(this->hasTextureLocation, this->material->hasTexture);
	glBindVertexArray(this->vao);

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
		glm::vec4 glowColor;
	} materialData = { glm::vec4(this->material->kd, ambient), glm::vec4(this->material->ks, this->material->ns), glm::vec4(this->material->glowColor, 1.0f)};
	
	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);

	glBufferData(GL_UNIFORM_BUFFER, sizeof(MaterialData), &materialData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, this->ubo);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
