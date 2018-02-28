#include "PhongLS.h"

#include "../../Error.h"

PhongLS::PhongLS() : ShaderProgram({ "Deferred Rendering/PhongLS.vs", GL_VERTEX_SHADER }, { "Deferred Rendering/PhongLS.fs", GL_FRAGMENT_SHADER })
{
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
	
	setUniform3fv("camPos", 1, &(this->getCamera()->getPosition()[0]));

	setTexture2D("positionTexture", 0, textures[0]);
	setTexture2D("normalTexture", 1, textures[1]);
	setTexture2D("kd_a_Texture", 2, textures[2]);
	setTexture2D("ks_ns_Texture", 3, textures[3]);
	setTexture2D("shadowMap", 4, textures[4]);
	
}

void PhongLS::updateLights()
{
	struct LightData
	{
		PointLight::PointLightData data[5];
		DirectionalLight::DirectionalLightData dirData;
	} lightData; 

	for (int i = 0; i < (int)this->pointLights.size(); i++)
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

	setUniform1i("nrOfPointLights", this->pointLights.size() > 5 ? 5 : this->pointLights.size());
}

void PhongLS::addPointLight(PointLight::PointLightData* data)
{
	this->pointLights.push_back(data);
}

void PhongLS::setDirectionalLight(DirectionalLight::DirectionalLightData * data)
{
	this->directionalLight = data;
}

void PhongLS::setShadowCamera(Camera* shadowCamera)
{
	this->shadowCamera = shadowCamera;
	setUniformMatrix4fv("shadowCamera", 1, false, &shadowCamera->getVP()[0][0]);
}

void PhongLS::setShadowSize(const glm::vec2 & shadowSize)
{
	setUniform2fv("shadowSize", 1, &shadowSize[0]);
}

void PhongLS::selfUpdateUniforms(Node * entity)
{
}
