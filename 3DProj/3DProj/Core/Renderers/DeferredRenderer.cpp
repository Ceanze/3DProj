#include "DeferredRenderer.h"

#include "../Display.h"

#include <vector>

#include "..\..\Error.h"

DeferredRenderer::DeferredRenderer(Display* display)
{
	this->gBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->gBuffer->createTextures(std::vector<FrameBuffer::FBO_ATTATCHMENT_TYPE>{ 
		FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_DEPTH_ATTACHMENT
	});

	this->lightningBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->lightningBuffer->createTextures(std::vector<FrameBuffer::FBO_ATTATCHMENT_TYPE>{
		FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_COLOR_ATTACHMENT
	});

	this->phongShader = new PhongLS();

	this->quadShader = new QuadShader();
	createQuad();
}

DeferredRenderer::~DeferredRenderer()
{
	delete this->lightningBuffer;
	delete this->gBuffer;
	delete this->quadShader;
	delete this->phongShader;
}

void DeferredRenderer::render(Node * node)
{
	this->gBuffer->bind();
	node->render();
	glBindVertexArray(0);
	glUseProgram(0);
	this->gBuffer->unbind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, this->lightningBuffer->getWidth(), this->lightningBuffer->getHeight());

	glUseProgram(this->phongShader->getID());
	this->phongShader->updateUniforms(this->gBuffer->getTextures(), this->gBuffer->getNumTextures());

	this->lightningBuffer->bind();
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
	this->lightningBuffer->unbind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, this->gBuffer->getWidth(), this->gBuffer->getHeight()); // Change this to display!

	glUseProgram(this->quadShader->getID());

	GLuint* quadTextures = new GLuint[3]{this->lightningBuffer->getTexture(0), this->lightningBuffer->getTexture(1), this->gBuffer->getTexture(2)};
	this->quadShader->updateUniforms(quadTextures, 3);
	delete[] quadTextures;

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

const FrameBuffer * DeferredRenderer::getGBuffer() const
{
	return this->gBuffer;
}

const FrameBuffer * DeferredRenderer::getLBuffer() const
{
	return this->lightningBuffer;
}

void DeferredRenderer::setCamera(Camera * camera)
{
	this->phongShader->setCamera(camera);
}

void DeferredRenderer::setLightPointer(LightComponent & component)
{
	component.setListOfLights(this->phongShader->getLights());
}

void DeferredRenderer::createQuad()
{
	static const GLfloat quadVertexBufferData[30] = {
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		-1.0f, 1.0f, 0.0f,		0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,		1.0f, 1.0f
	};

	glGenVertexArrays(1, &this->quadVAO);
	glBindVertexArray(this->quadVAO);
	glGenBuffers(1, &this->quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexBufferData), quadVertexBufferData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(nullptr));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));

	glBindVertexArray(0);
}
