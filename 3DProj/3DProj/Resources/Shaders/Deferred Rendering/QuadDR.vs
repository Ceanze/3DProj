#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTextureCoord;

out vec2 fragTextureCoord;

void main()
{
    fragTextureCoord = vertexTextureCoord;
    gl_Position = vec4(vertexPosition, 1.0);
}