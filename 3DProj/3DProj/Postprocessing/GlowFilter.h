#ifndef GLOW_FILTER_H
#define GLOW_FILTER_H

#include "../Core/GL Utils/FrameBuffer.h"
#include "BrightnessFilter.h"
#include "BlurFilter.h"

class GlowFilter
{
public:
	GlowFilter(unsigned int width, unsigned int height, BlurFilter* blurFilter);
	GlowFilter(unsigned int width, unsigned int height, float scale = 1.0f);
	virtual ~GlowFilter();

	void resize(unsigned int newWidth, unsigned int newHeight);
	void render(GLuint texture, GLuint quadVAO);
	void render(FrameBuffer* fb, GLuint quadVAO);

	FrameBuffer* getBrightnessBuffer();
	FrameBuffer* getFrameBuffer();
	GLuint getTexture();

private:
	BrightnessFilter* brightnessFilter;
	BlurFilter* blurFilter;
	bool useOwnBlur;
};

#endif