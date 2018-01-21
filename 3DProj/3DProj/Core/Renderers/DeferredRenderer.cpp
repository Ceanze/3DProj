#include "DeferredRenderer.h"

#include "../Display.h"

#include <vector>

#include "..\..\Error.h"

DeferredRenderer::DeferredRenderer(const Display* display)
{
	this->displayWidth = display->getWidth();
	this->displayHeight = display->getHeight();

	this->gBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->gBuffer->createTextures(std::vector<FrameBuffer::FBO_ATTATCHMENT_TYPE>{ 
		FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_DEPTH_ATTACHMENT
	});

	this->lightningBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->lightningBuffer->createTextures(std::vector<FrameBuffer::FBO_ATTATCHMENT_TYPE>{
		FrameBuffer::FBO_COLOR_ATTACHMENT, FrameBuffer::FBO_COLOR_ATTACHMENT
	});

	this->phongShader = new PhongLS();

	this->quadTextures = new GLuint[3];
	this->quadShader = new QuadShader();
	createQuad();
}

DeferredRenderer::~DeferredRenderer()
{
	delete this->lightningBuffer;
	delete this->gBuffer;
	delete this->quadShader;
	delete[] this->quadTextures;
	delete this->phongShader;
}

void DeferredRenderer::render(Node * node)
{
	renderGBuffer(node);
	renderLightBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, this->displayWidth, this->displayHeight); // Change this to display!

	glUseProgram(this->quadShader->getID());

	this->quadTextures[0] = this->lightningBuffer->getTexture(0);
	this->quadTextures[1] = this->lightningBuffer->getTexture(1);
	this->quadTextures[2] = this->gBuffer->getTexture(2);
	this->quadShader->updateUniforms(quadTextures, 3);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
}

void DeferredRenderer::resize(const Display * display)
{
	this->gBuffer->resize(display->getWidth(), display->getHeight());
	this->lightningBuffer->resize(display->getWidth(), display->getHeight());
	this->displayWidth = display->getWidth();
	this->displayHeight = display->getHeight();
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

PhongLS * DeferredRenderer::getPhongShader()
{
	return this->phongShader;
}

void DeferredRenderer::renderGBuffer(Node * node)
{
	this->gBuffer->bind();
	node->render();
	this->gBuffer->unbind();
}

void DeferredRenderer::renderLightBuffer()
{
	this->lightningBuffer->bind();

	glUseProgram(this->phongShader->getID());
	this->phongShader->updateUniforms(this->gBuffer->getTextures(), this->gBuffer->getNumTextures());

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);

	this->lightningBuffer->unbind();
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
