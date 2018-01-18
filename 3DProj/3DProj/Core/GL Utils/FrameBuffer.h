#ifndef FRAME_BUFFER_H
#define FRAME_BUFFER_H

#include <glew.h>

#include <vector>

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

	GLuint getTexture(unsigned int index);

	unsigned int getWidth() const;
	unsigned int getHeight() const;

private:
	void createTexture(FBO_ATTATCHMENT_TYPE attachment);

	GLuint fbo;
	GLuint* textures;
	std::vector<FBO_ATTATCHMENT_TYPE> attachments;
	unsigned int numTextures;
	unsigned int width, height;

	bool hasDepthAttachment;
	bool hasColorAttachment;
};

#endif
