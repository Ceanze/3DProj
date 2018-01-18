#version 330

in vec2 fragTextureCoord;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;
uniform sampler2D albedoTexture;
uniform sampler2D depthTexture;

out vec4 finalColor;

void main()
{
 
    if(fragTextureCoord.x < 0.5 && fragTextureCoord.y < 0.5) // BL
        finalColor = texture(albedoTexture, fragTextureCoord);
    else if(fragTextureCoord.x > 0.5 && fragTextureCoord.y < 0.5) // BR
        finalColor = texture(depthTexture, fragTextureCoord);
    else if(fragTextureCoord.x < 0.5 && fragTextureCoord.y > 0.5) // TL
        finalColor = texture(positionTexture, fragTextureCoord);
    else
        finalColor = texture(normalTexture, fragTextureCoord);
}