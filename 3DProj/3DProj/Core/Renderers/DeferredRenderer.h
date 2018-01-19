#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

#include "../../Entities/Node.h"
#include "../GL Utils/FrameBuffer.h"
#include "../../Shading/Deferred Rendering/QuadShader.h"
#include "../../Shading/Deferred Rendering/PhongLS.h"

class DeferredRenderer
{
public:
	DeferredRenderer(Display* display);
	virtual ~DeferredRenderer();

	void render(Node* node);

	const FrameBuffer* getGBuffer() const;
	const FrameBuffer* getLBuffer() const;

	void setCamera(Camera* camera);

private:
	void createQuad();

	QuadShader* quadShader;
	PhongLS* phongShader;


	FrameBuffer* gBuffer;
	FrameBuffer* lightningBuffer;

	GLuint quadVAO;
	GLuint quadVBO;
};

#endif
