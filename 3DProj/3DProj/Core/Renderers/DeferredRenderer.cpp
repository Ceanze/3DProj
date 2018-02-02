#include "DeferredRenderer.h"

#include "../Display.h"

#include <vector>

#include "..\..\Error.h"

DeferredRenderer::DeferredRenderer(Display* display)
{
	this->display = display;

	this->gBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->gBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{ 
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },
		{ FrameBuffer::FBO_DEPTH_ATTACHMENT, GL_RGBA16F }
	});

	this->lightingBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->lightingBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F}, {FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }
	});

	this->combineBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->combineBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }
	});

	this->shadowBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->shadowBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_DEPTH_ATTACHMENT, GL_RGBA16F }
	});

	this->brightnessFilter = new BrightnessFilter(display->getWidth(), display->getHeight());
	this->blurFilter = new BlurFilter(display->getWidth(), display->getHeight(), 0.5f);

	this->phongShader = new PhongLS();
	this->combineShader = new CombineShader();
	this->glowShader = new GlowShader();
	this->quadShader = new QuadShader();
	createQuad();
}

DeferredRenderer::~DeferredRenderer()
{
	delete this->gBuffer;
	delete this->quadShader;

	delete this->lightingBuffer;
	delete this->phongShader;

	delete this->combineBuffer;
	delete this->combineShader;

	delete this->brightnessFilter;
	delete this->blurFilter;

	delete this->glowShader;

	delete this->shadowBuffer;
}

void DeferredRenderer::render(Node * node)
{
	renderGBuffer(node);
	renderLightBuffer();
	renderCombineBuffer();
	renderGlowBlurOrNormal();
}

void DeferredRenderer::render(Node * node, Terrain * terrain)
{
	this->gBuffer->bind();
	node->render();
	terrain->render();
	this->gBuffer->unbind();
	renderLightBuffer();
	renderCombineBuffer();
	renderGlowBlurOrNormal();
}

void DeferredRenderer::resize(Display * display)
{
	this->display = display;
	this->gBuffer->resize(display->getWidth(), display->getHeight());
	this->lightingBuffer->resize(display->getWidth(), display->getHeight());
	this->combineBuffer->resize(display->getWidth(), display->getHeight());

	this->brightnessFilter->resize(display->getWidth(), display->getHeight());
	this->blurFilter->resize(display->getWidth(), display->getHeight());
}

const FrameBuffer * DeferredRenderer::getGBuffer() const
{
	return this->gBuffer;
}

const FrameBuffer * DeferredRenderer::getLBuffer() const
{
	return this->lightingBuffer;
}

const FrameBuffer * DeferredRenderer::getBrightnessBuffer() const
{
	return this->brightnessFilter->getFrameBuffer();
}

const FrameBuffer * DeferredRenderer::getBlurBuffer() const
{
	return this->blurFilter->getFrameBufferH();
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
	this->lightingBuffer->bind();
	
	glUseProgram(this->phongShader->getID());
	this->phongShader->updateUniforms(this->gBuffer->getTextures(), this->gBuffer->getNumTextures());

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);

	this->lightingBuffer->unbind();
}

void DeferredRenderer::renderCombineBuffer()
{
	this->combineBuffer->bind();

	glUseProgram(this->combineShader->getID());
	this->texturesTempArr[0] = this->lightingBuffer->getTexture(0);
	this->texturesTempArr[1] = this->lightingBuffer->getTexture(1);
	this->texturesTempArr[2] = this->gBuffer->getTexture(2);
	this->combineShader->updateUniforms(this->texturesTempArr, 3);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);

	this->combineBuffer->unbind();
}

void DeferredRenderer::renderGlowBlurOrNormal()
{
	static bool isBClicked = 0;
	static bool isBPressed = false;
	static bool isGClicked = 0;
	static bool isGPressed = false;

	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_B) != GLFW_PRESS)
	{
		if (isBPressed)
		{
			isBPressed = false;
			isBClicked ^= 1;
			isGClicked = false;
		}
	}
	else isBPressed = true;

	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_G) != GLFW_PRESS)
	{
		if (isGPressed)
		{
			isGPressed = false;
			isGClicked ^= 1;
			isBClicked = false;
		}
	}
	else isGPressed = true;

	if (isGClicked) // Glow effect
	{
		this->brightnessFilter->render(this->combineBuffer, this->quadVAO);
		this->blurFilter->render(this->brightnessFilter->getFrameBuffer(), this->quadVAO);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->glowShader->getID());
		this->texturesTempArr[0] = this->combineBuffer->getTexture();
		this->texturesTempArr[1] = this->blurFilter->getTexture();
		this->glowShader->updateUniforms(this->texturesTempArr, 1);

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	else if (isBClicked) // Blured
	{
		this->blurFilter->render(this->combineBuffer, this->quadVAO);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->quadShader->getID());
		this->texturesTempArr[0] = this->blurFilter->getTexture();
		this->quadShader->updateUniforms(this->texturesTempArr, 1);

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	else // Normal
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->quadShader->getID());
		this->quadShader->updateUniforms(this->combineBuffer->getTextures(), 1);

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
	}
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
