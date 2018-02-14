#include "BrightnessFilter.h"

BrightnessFilter::BrightnessFilter(unsigned int width, unsigned int height, float scale)
{
	this->scale = scale;
	this->brightnessShader = new BrightnessShader();
	this->frameBuffer = new FrameBuffer((unsigned int)(width*scale), (unsigned int)(height*scale));
	this->frameBuffer->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16 }
	});
}

BrightnessFilter::~BrightnessFilter()
{
	delete this->brightnessShader;
	delete this->frameBuffer;
}

void BrightnessFilter::resize(unsigned int newWidth, unsigned int newHeight)
{
	this->frameBuffer->resize((unsigned int)(newWidth*this->scale), (unsigned int)(newHeight*this->scale));
}

void BrightnessFilter::render(GLuint texture, GLuint quadVAO)
{
	this->frameBuffer->bind();

	glUseProgram(this->brightnessShader->getID());
	GLuint* textures = new GLuint[1];
	textures[0] = texture;
	this->brightnessShader->updateUniforms(textures, 1);
	delete[] textures;

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);

	this->frameBuffer->unbind();
}

void BrightnessFilter::render(FrameBuffer * fb, GLuint quadVAO)
{
	this->frameBuffer->bind();

	glUseProgram(this->brightnessShader->getID());
	this->brightnessShader->updateUniforms(fb->getTextures(), 1);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glUseProgram(0);

	this->frameBuffer->unbind();
}

FrameBuffer * BrightnessFilter::getFrameBuffer()
{
	return this->frameBuffer;
}

GLuint BrightnessFilter::getTexture()
{
	return this->frameBuffer->getTexture(0);
}