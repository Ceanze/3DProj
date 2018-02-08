#version 330

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexTangent;
layout(location = 3) in vec2 vertexUvs;

uniform mat4 wm;

out vec3 geometryPosition;
out vec3 geometryNormal;
out vec3 geometryTangent;
out vec2 geometryTextureCoord; 

void main()
{
    geometryPosition = (wm*vec4(vertexPosition, 1)).xyz;
    geometryNormal = (wm*vec4(vertexNormal, 0)).xyz;
    geometryTangent = (wm*vec4(vertexTangent, 0)).xyz;
    geometryTextureCoord = vertexUvs;
}