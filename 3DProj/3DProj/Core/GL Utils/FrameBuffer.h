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
		FBO_DEPTH_ATTACHMENT,
		FBO_DEPTH_ATTACHMENT_HIDDEN
	};

public:
	FrameBuffer(unsigned int width, unsigned int height);
	virtual ~FrameBuffer();

	void createTextures(const std::vector<std::pair<FBO_ATTATCHMENT_TYPE, GLuint>>& attachments);

	void resize(unsigned int width, unsigned int height);
	void bind();
	void unbind();

	void bindTexture(unsigned int index = 0);
	void unbindTexture();
	GLuint getTexture(unsigned int index = 0) const;
	GLuint* getTextures();
	unsigned int getNumTextures() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

private:
	void createFramebuffer(const std::vector<std::pair<FBO_ATTATCHMENT_TYPE, GLuint>>& attachments);
	void createRenderBuffer(const unsigned& width, const unsigned& height);

	GLuint fbo, rbo;
	Texture* textures;
	unsigned int width, height;

	bool hasDepthAttachment;
	bool hasDepthAttachmentHidden;
	bool hasColorAttachment;
};

#endif
