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

	this->kdALoc = glGetUniformLocation(this->getID(), "kd_a_Texture");
	if (kdALoc == -1)
		Error::printError("Could not find kd_a_Texture");

	this->ksNsLoc = glGetUniformLocation(this->getID(), "ks_ns_Texture");
	if (ksNsLoc == -1)
		Error::printError("Could not find ks_ns_Texture");

	this->camLoc = glGetUniformLocation(this->getID(), "camPos");
	if (camLoc == -1)
		Error::printError("Could not find camPos in PhongLS");

	this->nrOf = glGetUniformLocation(this->getID(), "nrOfPointLights");
	if (nrOf == -1)
		Error::printError("Couldn't find 'nrOfPointLights' in PhongLS");

	this->shadowCameraLoc = glGetUniformLocation(this->getID(), "shadowCamera");
	if(this->shadowCameraLoc == -1)
		Error::printError("Couldn't find 'shadowCamera' in PhongLS");

	this->shadowMapLoc = glGetUniformLocation(this->getID(), "shadowMap");
	if (shadowMapLoc == -1)
		Error::printError("Could not find 'shadowMap' in PhongLS");

	this->bindingPoint = 0;
	glGenBuffers(1, &this->ubo);
	GLuint uboIndex = glGetUniformBlockIndex(this->getID(), "Lights");
	glUniformBlockBinding(this->getID(), uboIndex, this->bindingPoint);
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

	glUniform1i(this->kdALoc, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]);

	glUniform1i(this->ksNsLoc, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, textures[3]);

	glUniform1i(this->shadowMapLoc, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, textures[4]);
}

void PhongLS::updateLights()
{
	struct LightData
	{
		PointLight::PointLightData data[5];
		DirectionalLight::DirectionalLightData dirData;
	} lightData; 

	for (int i = 0; i < this->pointLights.size(); i++)
	{
		if (i < 5)
		{
			lightData.data[i] = *this->pointLights[i];
		}
	}
	lightData.dirData = *this->directionalLight;

	glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(LightData), &lightData, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, this->bindingPoint, this->ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	
	glUniform1i(nrOf, this->pointLights.size() > 5 ? 5 : this->pointLights.size());
}

void PhongLS::addPointLight(PointLight::PointLightData* data)
{
	this->pointLights.push_back(data);
}

void PhongLS::setDirectionalLight(DirectionalLight::DirectionalLightData * data)
{
	this->directionalLight = data;
}

void PhongLS::setShadowCamera(const glm::mat4 & shadowCamera)
{
	glUniformMatrix4fv(this->shadowCameraLoc, 1, GL_FALSE, &shadowCamera[0][0]);
}

void PhongLS::selfUpdateUniforms(Node * entity)
{
}
