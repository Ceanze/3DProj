#ifndef COMBINE_SHADER_H
#define COMBINE_SHADER_H

#include "../ShaderProgram.h"

class CombineShader : public ShaderProgram
{
public:
	CombineShader();
	virtual ~CombineShader();

	void updateUniforms(GLuint* textures, unsigned nrOf);
private:
	void selfUpdateUniforms(Node* entity);

	GLint diffuseLoc, specularLoc, albedoLoc;

};

#endif
