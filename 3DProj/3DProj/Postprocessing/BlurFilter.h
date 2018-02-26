#ifndef BLUR_FILTER_H
#define BLUR_FILTER_H

#include "../Core/GL Utils/FrameBuffer.h"
#include "../Shading/Deferred Rendering/BlurShader.h"

class BlurFilter
{
public:
	BlurFilter(unsigned int width, unsigned int height, float blurSize = 1.0f, float scale = 1.0f);
	virtual ~BlurFilter();

	void resize(unsigned int newWidth, unsigned int newHeight);
	void render(FrameBuffer* fb, GLuint quadVAO);

	void setBlurSize(float blurSize);
	FrameBuffer* getFrameBufferV();
	FrameBuffer* getFrameBufferH();
	GLuint getTexture();

private:
	FrameBuffer* frameBufferV;
	FrameBuffer* frameBufferH;
	BlurShader* blurShader;
	float scale;
	float blurSize;
};

#endif
