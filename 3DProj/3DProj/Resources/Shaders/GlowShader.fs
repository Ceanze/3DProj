#version 330

in vec2 fragTextureCoord;

uniform sampler2D finalTexture;
uniform sampler2D glowTexture;

out vec4 finalColor;

void main()
{
    vec3 color = texture(finalTexture, fragTextureCoord).xyz;
    vec3 glow = texture(glowTexture, fragTextureCoord).xyz;
    finalColor = min(vec4(color + glow*2.0, 1.0f), 1.0);
}