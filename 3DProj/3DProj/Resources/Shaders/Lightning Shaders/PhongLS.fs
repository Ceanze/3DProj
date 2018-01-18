#version 330

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;

layout(location=0) out vec4 finalAmbient;
layout(location=1) out vec4 finalDiffuse;
layout(location=2) out vec4 finalSpecular;


in vec2 fragTextureCoord;

void main()
{
    vec2 test1 = fragTextureCoord;
    vec3 test = positionTexture * normalTexture * albedoTexture * depthTexture;
    finalAmbient = vec4(1.0f + test.x * 0.01f + test1.x * 0.01f, 0.0f, 0.0f , 1.0f);
    finalDiffuse = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    finalSpecular = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}