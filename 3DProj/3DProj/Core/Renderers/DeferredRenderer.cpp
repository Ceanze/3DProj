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

	this->lightningBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->lightningBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }, { FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }
	});

	this->combineBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->combineBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }
	});

	this->blurBuffer = new FrameBuffer(display->getWidth(), display->getHeight());
	this->blurBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16F }
	});

	this->phongShader = new PhongLS();

	this->combineTextures = new GLuint[3];
	this->combineShader = new CombineShader();

	this->blurShader = new BlurShader();

	this->quadShader = new QuadShader();
	createQuad();
}

DeferredRenderer::~DeferredRenderer()
{
	delete this->blurBuffer;
	delete this->lightningBuffer;
	delete this->gBuffer;
	delete this->quadShader;
	delete[] this->combineTextures;
	delete this->combineShader;
	delete this->combineBuffer;
	delete this->phongShader;
	delete this->blurShader;
}

void DeferredRenderer::render(Node * node)
{
	renderGBuffer(node);
	renderLightBuffer();
	renderCombineBuffer();
	renderBlur();
}

void DeferredRenderer::render(Node * node, Terrain * terrain)
{
	this->gBuffer->bind();
	node->render();
	terrain->render();
	this->gBuffer->unbind();
	renderLightBuffer();
	renderCombineBuffer();
	renderBlur();
}

void DeferredRenderer::resize(Display * display)
{
	this->display = display;
	this->gBuffer->resize(display->getWidth(), display->getHeight());
	this->lightningBuffer->resize(display->getWidth(), display->getHeight());
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

void DeferredRenderer::renderCombineBuffer()
{
	this->combineBuffer->bind();

	glUseProgram(this->combineShader->getID());

	this->combineTextures[0] = this->lightningBuffer->getTexture(0);
	this->combineTextures[1] = this->lightningBuffer->getTexture(1);
	this->combineTextures[2] = this->gBuffer->getTexture(2);
	this->combineShader->updateUniforms(combineTextures, 3);

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);

	this->combineBuffer->unbind();
}

void DeferredRenderer::renderBlur()
{
	static bool isBClicked = 0;
	static bool isBPressed = false;
	if (glfwGetKey(this->display->getWindowPtr(), GLFW_KEY_B) != GLFW_PRESS)
	{
		if (isBPressed)
		{
			isBPressed = false;
			isBClicked ^= 1;
		}
	}
	else isBPressed = true;

	if (isBClicked)
	{
		this->blurBuffer->bind();

		glUseProgram(this->blurShader->getID());
		this->blurShader->sendTextureSize(glm::vec2(this->combineBuffer->getWidth(), this->combineBuffer->getHeight()));
		this->blurShader->sendDirection({ 1.0f, 0.0f });
		this->blurShader->updateUniforms(this->combineBuffer->getTextures(), 1);

		glBindVertexArray(this->quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		glUseProgram(0);

		this->blurBuffer->unbind();
		
		// -------------------------------------------------------------------------------------

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->blurShader->getID());
		this->blurShader->sendTextureSize(glm::vec2(this->blurBuffer->getWidth(), this->blurBuffer->getHeight()));
		this->blurShader->sendDirection({ 0.0f, 1.0f });
		this->blurShader->updateUniforms(this->blurBuffer->getTextures(), 1);
	}
	else
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, this->display->getWidth(), this->display->getHeight());

		glUseProgram(this->quadShader->getID());
		this->quadShader->updateUniforms(this->combineBuffer->getTextures(), 1);
	}

	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);
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
