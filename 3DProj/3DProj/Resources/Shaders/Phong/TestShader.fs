#version 330

in vec2 fragUvs;
in vec3 fragNormal;
in vec3 fragPos;

out vec4 finalColor;

const vec3 lightPos = vec3(0.0, 2.0, 5.0);
const vec3 camPos = vec3(0.0, 2.0, 5.0);

uniform float time;
const float maxSonarLength = 100;

float puls(float x, float offset, float strength)
{
	return 1/(100*pow(x-offset, 2)+strength);
}

void main()
{
    vec3 normal = normalize(fragNormal);
	vec3 materialColor = min(vec3(0.5, 0.5, 0.5) + (fragNormal+1)*0.5*0.2 + vec3(fragUvs.x, fragUvs.y, 0.0f)*0.01, vec3(1.0, 1.0, 1.0));

	float len = length(fragPos);
	vec3 color = materialColor + vec3(0.0, 1.0, 0.0)*puls(len, time, 1.0);
	finalColor = vec4(min(color, vec3(1.0, 1.0, 1.0)), 1.0);
}