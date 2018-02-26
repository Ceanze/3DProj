#include "DeferredRenderer.h"

#include "../Display.h"

#include <vector>

#include "..\..\Error.h"

DeferredRenderer::DeferredRenderer(Display* display)
{
	this->display = display;

	this->gBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->gBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{ 
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Position
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Normal
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Albedo
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Material diffuse color and ambient factor
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Material specular color and specular exponent. 
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Glow color
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F },			// Normals for primitives
		{ FrameBuffer::FBO_DEPTH_ATTACHMENT_HIDDEN, GL_RGBA16F }	// Depth render buffer
	});

	this->lightingBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->lightingBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F}, // Diffuse (and ambient) light
		{FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }  // Specular light
	});

	this->combineBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->combineBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F } // Final image
	});
	this->combineBuffer->bindTexture(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->combineBuffer->unbindTexture();

	this->shadowResScale = 4.0f;
	this->shadowBuffer = new FrameBuffer((unsigned int)(display->getWidth()*this->shadowResScale), (unsigned int)(display->getHeight()*this->shadowResScale));
	this->shadowBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_DEPTH_ATTACHMENT, GL_RGBA16F } // Depth from shadow camera.
	});
	
	this->blurIntensity = 5.0;
	this->glowIntensity = 8.0;
	this->blurFilter = new BlurFilter(display->getWidth(), display->getHeight(), this->blurIntensity, 0.6f);
	this->blurFilter2 = new BlurFilter(display->getWidth(), display->getHeight(), 1.0f, 0.3f);
	this->glowFilter = new GlowFilter(display->getWidth(), display->getHeight(), this->blurFilter);

	this->phongShader = new PhongLS();
	this->combineShader = new CombineShader();
	this->glowShader = new GlowShader();
	this->quadShader = new QuadShader();
	this->shadowShader = new ShadowShader();
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

	delete this->glowFilter;
	delete this->blurFilter;
	delete this->blurFilter2;

	delete this->glowShader;

	delete this->shadowBuffer;
	delete this->shadowShader;
}

void DeferredRenderer::render(Node * node, bool useWireframe)
{
	renderGBuffer(node, useWireframe);

	this->shadowBuffer->bind();
	glUseProgram(this->shadowShader->getID());
	node->render(this->shadowShader);
	glUseProgram(0);
	this->shadowBuffer->unbind();

	renderLightBuffer();
	renderCombineBuffer();
	renderGlowBlurOrNormal();
}

void DeferredRenderer::render(Node * node, Terrain * terrain, bool useWireframe)
{
	this->gBuffer->bind();
	if (useWireframe)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}
	node->render();
	terrain->render();
	if (useWireframe)
	{
		glDisable(GL_BLEND);
	}
	this->gBuffer->unbind();

	this->shadowBuffer->bind();
	glUseProgram(this->shadowShader->getID());
	node->render(this->shadowShader);
	glUseProgram(0);
	this->shadowBuffer->unbind();

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

	this->glowFilter->resize(display->getWidth(), display->getHeight());
	this->blurFilter->resize(display->getWidth(), display->getHeight());
	this->blurFilter2->resize(display->getWidth(), display->getHeight());
	this->shadowBuffer->resize((unsigned int)(display->getWidth()*this->shadowResScale), (unsigned int)(display->getHeight()*this->shadowResScale));
}

const FrameBuffer * DeferredRenderer::getGBuffer() const
{
	return this->gBuffer;
}

const FrameBuffer * DeferredRenderer::getShadowBuffer() const
{
	return this->shadowBuffer;
}

const FrameBuffer * DeferredRenderer::getLBuffer() const
{
	return this->lightingBuffer;
}

const FrameBuffer * DeferredRenderer::getBrightnessBuffer() const
{
	return this->glowFilter->getBrightnessBuffer();
}

const FrameBuffer * DeferredRenderer::getBlurBuffer() const
{
	return this->blurFilter->getFrameBufferH();
}

void DeferredRenderer::setCamera(Camera * camera)
{
	this->phongShader->setCamera(camera);
}

void DeferredRenderer::setShadowCamera(Camera * shadowCamera)
{
	this->shadowShader->setCamera(shadowCamera);
}

PhongLS * DeferredRenderer::getPhongShader()
{
	return this->phongShader;
}

void DeferredRenderer::renderGBuffer(Node * node, bool useWireframe)
{
	this->gBuffer->bind();
	if (useWireframe)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	}
	node->render();
	if (useWireframe)
	{
		glDisable(GL_BLEND);
	}
	this->gBuffer->unbind();
}

void DeferredRenderer::renderLightBuffer()
{
	this->lightingBuffer->bind();

	glUseProgram(this->phongShader->getID());
	this->texturesTempArr[0] = this->gBuffer->getTexture(0);
	this->texturesTempArr[1] = this->gBuffer->getTexture(1);
	this->texturesTempArr[2] = this->gBuffer->getTexture(3);
	this->texturesTempArr[3] = this->gBuffer->getTexture(4);
	this->texturesTempArr[4] = this->shadowBuffer->getTexture(0);
	this->texturesTempArr[5] = this->gBuffer->getTexture(6);
	this->phongShader->updateUniforms(this->texturesTempArr, 6);

	this->phongShader->setShadowCamera(this->shadowShader->getCamera());
	this->phongShader->setShadowSize(glm::vec2(this->shadowBuffer->getWidth(), this->shadowBuffer->getHeight()));

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
	this->combineShader->updateUniforms(this->texturesTempArr, 4);

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
		this->blurFilter->setBlurSize(this->glowIntensity);
		this->glowFilter->render(this->gBuffer->getTexture(5), this->quadVAO);
		this->blurFilter2->render(this->glowFilter->getFrameBuffer(), this->quadVAO);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->glowShader->getID());
		this->texturesTempArr[0] = this->combineBuffer->getTexture();
		this->texturesTempArr[1] = this->blurFilter2->getTexture();
		this->glowShader->updateUniforms(this->texturesTempArr, 1);

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);
	}
	else if (isBClicked) // Blured
	{
		this->blurFilter->setBlurSize(this->blurIntensity);
		this->blurFilter->render(this->combineBuffer, this->quadVAO);
		this->blurFilter2->render(this->blurFilter->getFrameBufferH(), this->quadVAO);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->quadShader->getID());
		this->texturesTempArr[0] = this->blurFilter2->getTexture();
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
