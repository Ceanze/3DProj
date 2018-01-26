#version 330

in vec2 fragTextureCoord;

uniform sampler2D finalTexture;

out vec4 finalColor;

void main()
{
    finalColor = texture(finalTexture, fragTextureCoord);
}