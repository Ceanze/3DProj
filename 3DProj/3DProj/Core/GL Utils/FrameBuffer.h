#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glew.h>

#include <vector>

#include "Texture.h"

class FrameBuffer
{
public:
	enum FBO_ATTATCHMENT_TYPE
	{
		FBO_COLOR_ATTACHMENT,
		FBO_DEPTH_ATTACHMENT
	};

public:
	FrameBuffer(unsigned int width, unsigned int height);
	virtual ~FrameBuffer();

	void createTextures(const std::vector<FBO_ATTATCHMENT_TYPE>& attachments);

	void resize(unsigned int width, unsigned int height);
	void bind();
	void unbind();

	GLuint getTexture(unsigned int index) const;
	GLuint* getTextures();
	unsigned int getNumTextures() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

private:
	void createFramebuffer(const std::vector<FBO_ATTATCHMENT_TYPE>& attachments);

	GLuint fbo;
	Texture* textures;
	unsigned int width, height;

	bool hasDepthAttachment;
	bool hasColorAttachment;
};

#endif
