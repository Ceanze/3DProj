#include "GlowFilter.h"

GlowFilter::GlowFilter(unsigned int width, unsigned int height, BlurFilter * blurFilter)
{
	this->brightnessFilter = new BrightnessFilter(width, height);
	this->blurFilter = blurFilter;
	this->useOwnBlur = false;
}

GlowFilter::GlowFilter(unsigned int width, unsigned int height, float scale)
{
	this->brightnessFilter = new BrightnessFilter(width, height);
	this->blurFilter = new BlurFilter(width, height, scale);
	this->useOwnBlur = true;
}

GlowFilter::~GlowFilter()
{
	delete this->brightnessFilter;
	if(this->useOwnBlur)
		delete this->blurFilter;
}

void GlowFilter::resize(unsigned int newWidth, unsigned int newHeight)
{
	this->brightnessFilter->resize(newWidth, newHeight);
	if(this->useOwnBlur)
		this->blurFilter->resize(newWidth, newHeight);
}

void GlowFilter::render(GLuint texture, GLuint quadVAO)
{
	this->brightnessFilter->render(texture, quadVAO);
	this->blurFilter->render(this->brightnessFilter->getFrameBuffer(), quadVAO);
}

void GlowFilter::render(FrameBuffer * fb, GLuint quadVAO)
{
	this->brightnessFilter->render(fb, quadVAO);
	this->blurFilter->render(this->brightnessFilter->getFrameBuffer(), quadVAO);
}

FrameBuffer * GlowFilter::getBrightnessBuffer()
{
	return this->brightnessFilter->getFrameBuffer();
}

FrameBuffer * GlowFilter::getFrameBuffer()
{
	return this->blurFilter->getFrameBufferH();
}

GLuint GlowFilter::getTexture()
{
	return this->blurFilter->getTexture();
}
