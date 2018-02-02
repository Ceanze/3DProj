#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 depthMat;

void main()
{
    gl_Position = depthMat*vec4(vertexPosition, 1.0);
}