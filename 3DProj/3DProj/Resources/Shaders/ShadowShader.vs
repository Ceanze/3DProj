#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 wm;
uniform mat4 camera;

void main()
{
    gl_Position = camera*wm*vec4(vertexPosition, 1.0);
}