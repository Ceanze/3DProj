#ifndef BRIGHTNESS_FILTER_H
#define BRIGHTNESS_FILTER_H

#include "../Core/GL Utils/FrameBuffer.h"
#include "../Shading/BrightnessShader.h"

class BrightnessFilter
{
public:
	BrightnessFilter(unsigned int width, unsigned int height, float scale = 1.0f);
	virtual ~BrightnessFilter();

	void resize(unsigned int newWidth, unsigned int newHeight);
	void render(FrameBuffer* fb, GLuint quadVAO);

	FrameBuffer* getFrameBuffer();
	GLuint getTexture();

private:
	FrameBuffer* frameBuffer;
	BrightnessShader* brightnessShader;
	float scale;
};

#endif
