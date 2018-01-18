#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
{
	this->width = width;
	this->height = height;
	this->numTextures = 0;
	this->textures = nullptr;
	this->hasColorAttachment = false;
	this->hasDepthAttachment = false;
	glGenFramebuffers(1, &this->fbo);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &this->fbo);
	glDeleteTextures(this->attachments.size(), this->textures);
	delete[] this->textures;
}

void FrameBuffer::createTextures(const std::vector<FBO_ATTATCHMENT_TYPE>& attachments)
{
	this->textures = new GLuint[attachments.size()];
	glGenTextures(attachments.size(), this->textures);
	for (unsigned int i = 0; i < attachments.size(); i++)
	{
		this->attachments.push_back(attachments[i]);
		if (attachments[i] == FBO_COLOR_ATTACHMENT)
			this->hasColorAttachment = true;
		if (attachments[i] == FBO_DEPTH_ATTACHMENT)
			this->hasDepthAttachment = true;
		createTexture(attachments[i]);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	GLenum* drawBuffers = new GLenum[attachments.size()];
	for (unsigned int i = 0; i < attachments.size(); i++)
	{
		drawBuffers[i] = attachments[i] == FBO_COLOR_ATTACHMENT ? GL_COLOR_ATTACHMENT0 + i : GL_NONE;

		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachments[i] == FBO_COLOR_ATTACHMENT ? GL_COLOR_ATTACHMENT0+i : GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D, this->textures[i], 0);
	}

	glDrawBuffers(attachments.size(), drawBuffers);
	delete[] drawBuffers;

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void FrameBuffer::resize(unsigned int width, unsigned int height)
{
	for (unsigned int i = 0; i < this->attachments.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0,
			(attachments[i] == FBO_DEPTH_ATTACHMENT) ? GL_DEPTH_COMPONENT24 : GL_RGBA32F,
			width, height, 0,
			(attachments[i] == FBO_DEPTH_ATTACHMENT) ? GL_DEPTH_COMPONENT : GL_RGB,
			GL_FLOAT, 0);
	}
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

GLuint FrameBuffer::getTexture(unsigned int index)
{
	return this->textures[index];
}

void FrameBuffer::createTexture(FBO_ATTATCHMENT_TYPE attachment)
{
	glBindTexture(GL_TEXTURE_2D, this->textures[this->numTextures]);
	this->numTextures++;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0,
		(attachment == FBO_DEPTH_ATTACHMENT) ? GL_DEPTH_COMPONENT24 : GL_RGB32F,
		getWidth(), getHeight(), 0,
		(attachment == FBO_DEPTH_ATTACHMENT) ? GL_DEPTH_COMPONENT : GL_RGB,
		GL_FLOAT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int FrameBuffer::getWidth() const
{
	return this->width;
}

unsigned int FrameBuffer::getHeight() const
{
	return this->height;
}
