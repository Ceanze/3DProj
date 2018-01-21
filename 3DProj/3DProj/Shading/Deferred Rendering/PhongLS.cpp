#include "PhongLS.h"

#include "../../Error.h"

PhongLS::PhongLS() : ShaderProgram({ "Deferred Rendering/PhongLS.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/PhongLS.fs", GL_FRAGMENT_SHADER })
{
	this->positionLoc = glGetUniformLocation(this->getID(), "positionTexture");
	if (positionLoc == -1)
		Error::printError("Could not find positionLoc");

	this->normalLoc = glGetUniformLocation(this->getID(), "normalTexture");
	if (normalLoc == -1)
		Error::printError("Could not find normalLoc");

	this->camLoc = glGetUniformLocation(this->getID(), "camPos");
	if (camLoc == -1)
		Error::printError("Could not find camPos in PhongLS");

	this->nrOf = glGetUniformLocation(this->getID(), "nrOfPointLights");
	if (nrOf == -1)
		Error::printError("Couldn't find 'nrOfPointLights' in PhongLS");

	this->bindingPoint = 0;
	glGenBuffers(1, &this->ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glUniformBlockBinding(this->getID(), this->ubo, this->bindingPoint);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

PhongLS::~PhongLS()
{
}

void PhongLS::updateUniforms(GLuint* textures, unsigned nrOf)
{
	this->updateLights();
	
	glUniform3fv(this->camLoc, 1, &(this->getCamera()->getPosition()[0]));

	glUniform1i(this->positionLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glUniform1i(this->normalLoc, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
}

void PhongLS::updateLights()
{
	struct LightData
	{
		PointLight::PointLightData data[5];
	}lightData; 

	for (int i = 0; i < this->pointLights.size(); i++)
	{
		if (i < 5)
		{
			lightData.data[i] = *this->pointLights[i];
		}
	}

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData), &lightData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, this->ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	
	glUniform1i(nrOf, 1);
}

void PhongLS::addPointLight(PointLight::PointLightData* data)
{
	this->pointLights.push_back(data);
}

void PhongLS::selfUpdateUniforms(Node * entity)
{
}
