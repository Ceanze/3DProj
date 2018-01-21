#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

#include "../../Entities/Node.h"
#include "../GL Utils/FrameBuffer.h"
#include "../../Shading/Deferred Rendering/QuadShader.h"
#include "../../Shading/Deferred Rendering/PhongLS.h"
#include "../../Entities/Components/Lightning/LightComponent.h"

class DeferredRenderer
{
public:
	DeferredRenderer(const Display* display);
	virtual ~DeferredRenderer();

	void render(Node* node);

	void resize(const Display* display);

	const FrameBuffer* getGBuffer() const;
	const FrameBuffer* getLBuffer() const;

	void setCamera(Camera* camera);
	void setLightPointer(LightComponent& component);

private:
	void renderGBuffer(Node* node);
	void renderLightBuffer();
	void createQuad();

	QuadShader* quadShader;
	PhongLS* phongShader;

	GLuint* quadTextures;

	FrameBuffer* gBuffer;
	FrameBuffer* lightningBuffer;

	GLuint quadVAO;
	GLuint quadVBO;

	unsigned int displayWidth, displayHeight;
};

#endif
