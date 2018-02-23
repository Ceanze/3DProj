#include "FrameBuffer.h"

#include "../../Error.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->textures = nullptr;
	this->hasColorAttachment = false;
	this->hasDepthAttachment = false;
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &this->fbo);
	delete this->textures;
}

void FrameBuffer::createTextures(const std::vector<std::pair<FBO_ATTATCHMENT_TYPE, GLuint>>& attachments)
{
	std::vector<TextureInfo> textureInfos;
	for (unsigned int i = 0; i < attachments.size(); i++)
	{
		textureInfos.push_back(TextureInfo(this->width, this->height, nullptr, GL_FLOAT, 
			(attachments[i].first == FBO_DEPTH_ATTACHMENT) ? GL_DEPTH_COMPONENT : GL_RGB,
			(attachments[i].first == FBO_DEPTH_ATTACHMENT) ? GL_DEPTH_COMPONENT24 : attachments[i].second));

		if (attachments[i].first == FBO_COLOR_ATTACHMENT)
			this->hasColorAttachment = true;
		if (attachments[i].first == FBO_DEPTH_ATTACHMENT)
			this->hasDepthAttachment = true;
	}
	this->textures = new Texture(textureInfos);

	createFramebuffer(attachments);
}

void FrameBuffer::resize(unsigned int width, unsigned int height)
{
	this->textures->recreate(width, height, nullptr);
	this->width = width;
	this->height = height;
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glViewport(0, 0, getWidth(), getHeight());

	glClearColor(0, 0, 0, 1);
	glClear((this->hasColorAttachment?GL_COLOR_BUFFER_BIT:0) | (this->hasDepthAttachment ? GL_DEPTH_BUFFER_BIT : 0));
}

void FrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTexture(unsigned int index)
{
	this->textures->bind(index);
}

void FrameBuffer::unbindTexture()
{
	this->textures->unbind();
}

GLuint FrameBuffer::getTexture(unsigned int index) const
{
	return this->textures->getTexture(index);
}

GLuint * FrameBuffer::getTextures()
{
	return this->textures->getTextures();
}

unsigned int FrameBuffer::getNumTextures() const
{
	return this->textures->getNumTextures();
}

unsigned int FrameBuffer::getWidth() const
{
	return this->width;
}

unsigned int FrameBuffer::getHeight() const
{
	return this->height;
}

void FrameBuffer::createFramebuffer(const std::vector<std::pair<FBO_ATTATCHMENT_TYPE, GLuint>>& attachments)
{
	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	GLenum* drawBuffers = new GLenum[attachments.size()];
	for (unsigned int i = 0; i < attachments.size(); i++)
	{
		drawBuffers[i] = attachments[i].first == FBO_COLOR_ATTACHMENT ? GL_COLOR_ATTACHMENT0 + i : GL_NONE;

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, 
			attachments[i].first == FBO_COLOR_ATTACHMENT ? GL_COLOR_ATTACHMENT0 + i : GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, this->textures->getTexture(i), 0);
	}

	glDrawBuffers(attachments.size(), drawBuffers);
	delete[] drawBuffers;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Error::printError("Failed to attach and draw texture to the framebuffer!");

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
