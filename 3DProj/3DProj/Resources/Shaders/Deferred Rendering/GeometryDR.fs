#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec3 fragTangent;
in vec2 fragTextureCoord; 
in vec3 dist;

layout(std140) uniform Material
{
	vec4 kd_a;
    vec4 ks_ns;
	vec4 glowColor;
} material;

layout(location=0) out vec4 finalPosition;
layout(location=1) out vec4 finalNormal;
layout(location=2) out vec4 finalAlbedo;
layout(location=3) out vec4 finalKd_a;
layout(location=4) out vec4 finalKs_ns;
layout(location=5) out vec4 finalGlowColor;

uniform int useNormalMap;
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform bool useWireframe;

void main()
{
	vec3 normal = normalize(fragNormal);
	if(useNormalMap == 1.0)
	{
		vec3 tangent = normalize(fragTangent);
		vec3 bitangent = cross(tangent, normal);
		mat3 tbn = mat3(tangent, bitangent, normal); // Tangent space to world space.
		normal = normalize((tbn*(texture(normalMap, fragTextureCoord).xyz*2.0 - 1.0)));
	}

	const float epsilon = 0.025;
	if(useWireframe)
	{
		if(dist.x < epsilon || dist.y < epsilon || dist.z < epsilon)
		{
			finalAlbedo = vec4(1.0, 1.0, 1.0, 1.0);
			finalNormal = vec4(normalize(fragNormal), 1.0);
		}
	}
	else
	{
    	finalNormal = vec4(normal, 1.0);
    	finalAlbedo = texture(albedoMap, fragTextureCoord);
	}

    finalPosition = vec4(fragPosition, 1.0);

	finalKd_a = material.kd_a;
	finalKs_ns = material.ks_ns;
	finalGlowColor = material.glowColor;
}