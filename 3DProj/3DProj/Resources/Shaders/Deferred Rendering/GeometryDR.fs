#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTextureCoord; 

layout(location=0) out vec4 finalPosition;
layout(location=1) out vec4 finalNormal;
layout(location=2) out vec4 finalAlbedo;

void main()
{
    finalAlbedo = vec4(fragTextureCoord, 0.5, 1.0);
    finalNormal = vec4(normalize(fragNormal), 1.0);
    finalPosition = vec4(fragPosition, 1.0);
}