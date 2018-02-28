#include "BlurFilter.h"
#include "GlowFilter.h"

BlurFilter::BlurFilter(unsigned int width, unsigned int height, float blurSize, float scale)
{
	this->scale = scale;
	this->blurSize = blurSize;
	this->blurShader = new BlurShader();

	// Framebuffer to blur verticaly.
	this->frameBufferV = new FrameBuffer((unsigned int)(width*scale), (unsigned int)(height*scale));
	this->frameBufferV->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16 }
	});
	this->frameBufferV->bindTexture(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->frameBufferV->unbindTexture();
	
	// Framebuffer to blur horizontaly.
	this->frameBufferH = new FrameBuffer((unsigned int)(width*scale), (unsigned int)(height*scale));
	this->frameBufferH->createTextures(std::vector<std::pair<FrameBuffer::FBO_ATTATCHMENT_TYPE, GLuint>>{
		{ FrameBuffer::FBO_COLOR_ATTACHMENT, GL_RGBA16 }
	});
	this->frameBufferH->bindTexture(0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	this->frameBufferH->unbindTexture();
}

BlurFilter::~BlurFilter()
{
	delete this->blurShader;
	delete this->frameBufferV;
	delete this->frameBufferH;
}

void BlurFilter::resize(unsigned int newWidth, unsigned int newHeight)
{
	this->frameBufferV->resize((unsigned int)(newWidth*this->scale), (unsigned int)(newHeight*this->scale));
	this->frameBufferH->resize((unsigned int)(newWidth*this->scale), (unsigned int)(newHeight*this->scale));
}

void BlurFilter::render(FrameBuffer* fb, GLuint quadVAO)
{
	glUseProgram(this->blurShader->getID());
	glBindVertexArray(quadVAO);
	
	// Scale down to match the size of the filter. 
	this->frameBufferH->bind();

	this->blurShader->sendTextureSize(glm::vec2(fb->getWidth(), fb->getHeight()));
	this->blurShader->sendDirection({ 0.0f, 0.0f }); // No blur, only scale down to match the size of the filter.
	this->blurShader->updateUniforms(fb->getTextures(), 1);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->frameBufferH->unbind();
	
	// Vertical blur
	this->frameBufferV->bind();

	this->blurShader->sendTextureSize(glm::vec2(this->frameBufferH->getWidth(), this->frameBufferH->getHeight()));
	this->blurShader->sendDirection({ 0.0f, blurSize });
	this->blurShader->updateUniforms(this->frameBufferH->getTextures(), 1);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->frameBufferV->unbind();

	// Horizontal blur
	this->frameBufferH->bind();

	glUseProgram(this->blurShader->getID());
	this->blurShader->sendTextureSize(glm::vec2(this->frameBufferV->getWidth(), this->frameBufferV->getHeight()));
	this->blurShader->sendDirection({ blurSize, 0.0f });
	this->blurShader->updateUniforms(this->frameBufferV->getTextures(), 1);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	this->frameBufferH->unbind();

	glBindVertexArray(0);
	glUseProgram(0);
}

void BlurFilter::setBlurSize(float blurSize)
{
	this->blurSize = blurSize;
}

FrameBuffer * BlurFilter::getFrameBufferV()
{
	return this->frameBufferV;
}

FrameBuffer * BlurFilter::getFrameBufferH()
{
	return this->frameBufferH;
}

GLuint BlurFilter::getTexture()
{
	return this->frameBufferH->getTexture(0);
}
