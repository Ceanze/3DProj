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

	this->quadShader = new QuadShader();
	createQuad();
}

DeferredRenderer::~DeferredRenderer()
{
	delete this->gBuffer;
	delete this->quadShader;
}

void DeferredRenderer::render(Node * node)
{
	this->gBuffer->bind();
	node->render();
	this->gBuffer->unbind();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, this->gBuffer->getWidth(), this->gBuffer->getHeight());


	glUseProgram(this->quadShader->getID());

	glUniform1d(0, 0);

	GLint positionLoc = glGetUniformLocation(this->quadShader->getID(), "positionTexture");
	if (positionLoc == -1)
		Error::printError("Could not find positionLoc");

	GLint normalLoc = glGetUniformLocation(this->quadShader->getID(), "normalTexture");
	if (positionLoc == -1)
		Error::printError("Could not find normalLoc");

	GLint albedoLoc = glGetUniformLocation(this->quadShader->getID(), "albedoTexture");
	if (positionLoc == -1)
		Error::printError("Could not find albedoLoc");

	GLint depthLoc = glGetUniformLocation(this->quadShader->getID(), "depthTexture");
	if (positionLoc == -1)
		Error::printError("Could not find depthLoc");

	
	glUniform1i(positionLoc, this->gBuffer->getTexture(0));
	glUniform1i(normalLoc, this->gBuffer->getTexture(1));
	glUniform1i(albedoLoc, this->gBuffer->getTexture(2));
	glUniform1i(depthLoc, this->gBuffer->getTexture(3));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->gBuffer->getTexture(0));

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->gBuffer->getTexture(1));

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->gBuffer->getTexture(2));

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, this->gBuffer->getTexture(3));
	
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
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
