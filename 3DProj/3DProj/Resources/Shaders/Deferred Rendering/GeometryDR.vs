#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUvs;

uniform mat4 camera;
uniform mat4 wm;

out vec3 fragPosition;
out vec3 fragNormal;
out vec2 fragTextureCoord; 

void main()
{
    fragPosition = (wm*vec4(vertexPosition, 1)).xyz;
    fragNormal = (wm*vec4(vertexNormal, 0)).xyz;
    fragTextureCoord = vertexUvs;
    gl_Position = camera*vec4(fragPosition, 1);
}