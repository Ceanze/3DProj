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
    vec3 test = texture(positionTexture, fragTextureCoord).xyz * texture(normalTexture, fragTextureCoord).xyz * texture(albedoTexture, fragTextureCoord).xyz * texture(depthTexture, fragTextureCoord).xyz;
    finalAmbient = vec4(1.0 +  test.x * 0.01, 0.0, 0.0, 1.0);
    finalDiffuse = vec4(0.0, 1.0, 0.0, 1.0);
    finalSpecular = vec4(1.0, 0.0, 0.0, 1.0);
}