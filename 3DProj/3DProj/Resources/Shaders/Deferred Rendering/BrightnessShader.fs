#version 330

in vec2 fragTextureCoord;

uniform sampler2D finalTexture;

out vec4 finalColor;

void main()
{
    vec3 color = texture(finalTexture, fragTextureCoord).xyz;
    float intensity = color.r*0.2126 + color.g*0.7152 + color.b*0.0722;
    finalColor = min(vec4(color*intensity, 1.0f), 1.0);
}