#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

#include "../../Entities/Node.h"
#include "../GL Utils/FrameBuffer.h"
#include "../../Shading/Deferred Rendering/QuadShader.h"

class DeferredRenderer
{
public:
	DeferredRenderer(Display* display);
	virtual ~DeferredRenderer();

	void render(Node* node);

	const FrameBuffer* getGBuffer() const;

private:
	void createQuad();

	QuadShader* quadShader;
	FrameBuffer* gBuffer;
	GLuint quadVAO;
	GLuint quadVBO;
};

#endif
