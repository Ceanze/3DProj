#ifndef BLUR_SHADER_H
#define BLUR_SHADER_H

#include "../ShaderProgram.h"

class BlurShader : public ShaderProgram
{
public:
	BlurShader();
	virtual ~BlurShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);
	void sendTextureSize(const glm::vec2& size);
	void sendDirection(const glm::vec2& dir);

private:
	void selfUpdateUniforms(Node* entity);
};

#endif
