#version 330

in vec2 fragUvs;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 finalColor;

const vec3 lightPos = vec3(0.0, 2.0, 5.0);
const vec3 camPos = vec3(0.0, 2.0, 5.0);

void main()
{
    vec3 normal = normalize(fragNormal);
	vec3 materialColor = min(vec3(0.5, 0.5, 0.5) + vec3(fragUvs.x, fragUvs.y, 0.0f), vec3(1.0, 1.0, 1.0));

    // Ambient part
    vec3 ambientFactor = vec3(0.2, 0.2, 0.2);
    vec3 ambient = ambientFactor*materialColor;

    vec3 color = ambient;

    // Diffuse part
    vec3 fragToLight = normalize(lightPos-fragPos.xyz);
    float diffuseFactor = max(dot(normal, fragToLight), 0.0);
    vec3 diffuse = diffuseFactor*materialColor*0.8;

    // Specular part
    vec3 specularColor = vec3(1.0, 1.0, 1.0);
    float s = 80.0;
    vec3 r = reflect(normal, fragToLight);
    vec3 v = normalize(camPos - fragPos.xyz);
    float specularFactor = pow(max(dot(r, v), 0.0), s);
    vec3 specular = specularFactor*specularColor;

    color += diffuse + specular; 

	finalColor = vec4(min(color, vec3(1.0, 1.0, 1.0)), 1.0);
}