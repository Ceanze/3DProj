#version 440

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUvs;

uniform mat4 mw;
uniform mat4 camera;

out vec2 fragUvs;
out vec3 fragNormal;
out vec3 fragPos;

void main() 
{
	fragPos = (mw*vec4(vertexPosition, 1.0)).xyz;
	fragUvs = vertexUvs;
	fragNormal = (mw*vec4(vertexNormal, 0.0)).xyz;
	gl_Position = camera*vec4(fragPos, 1.0);
}