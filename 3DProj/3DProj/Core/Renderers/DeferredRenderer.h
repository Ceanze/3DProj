#ifndef DEFERRED_RENDERER_H
#define DEFERRED_RENDERER_H

#include "../../Entities/Node.h"
#include "../GL Utils/FrameBuffer.h"
#include "../../Shading/Deferred Rendering/QuadShader.h"
#include "../../Shading/Deferred Rendering/PhongLS.h"
#include "../../Shading/Deferred Rendering/CombineShader.h"
#include "../../Shading/GlowShader.h"
#include "../../Shading/ShadowShader.h"
#include "../../Postprocessing/BlurFilter.h"
#include "../../Postprocessing/BrightnessFilter.h"
#include "../../Entities/Components/Lightning/LightComponent.h"
#include "../../Terrain/Terrain.h"


class DeferredRenderer
{
public:
	DeferredRenderer(Display* display);
	virtual ~DeferredRenderer();

	void render(Node* node);
	void render(Node* node, Terrain* terrain);

	void resize(Display* display);

	const FrameBuffer* getGBuffer() const;
	const FrameBuffer* getShadowBuffer() const;
	const FrameBuffer* getLBuffer() const;
	const FrameBuffer* getBrightnessBuffer() const;
	const FrameBuffer* getBlurBuffer() const;

	void setCamera(Camera* camera);
	void setShadowCamera(Camera* shadowCamera);

	PhongLS* getPhongShader();

private:
	void renderGBuffer(Node* node);
	void renderLightBuffer();
	void renderCombineBuffer();
	void renderGlowBlurOrNormal();
	
	void createQuad();

	QuadShader* quadShader;
	PhongLS* phongShader;
	CombineShader* combineShader;
	GlowShader* glowShader;
	ShadowShader* shadowShader;

	GLuint texturesTempArr[5];

	FrameBuffer* gBuffer;
	FrameBuffer* lightingBuffer;
	FrameBuffer* combineBuffer;
	FrameBuffer* shadowBuffer;

	BrightnessFilter* brightnessFilter;
	BlurFilter* blurFilter;

	GLuint quadVAO;
	GLuint quadVBO;

	Display* display;
};

#endif
