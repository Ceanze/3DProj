#version 330

in vec2 fragTextureCoord;

uniform sampler2D finalTexture;
uniform vec2 textureResolution;
uniform vec2 dir;

out vec4 finalColor;

const float blurArr[4] = float[4](0.002291, 0.023226, 0.092651, 0.146768);

void main()
{
    vec3 outColor = vec3(0.0, 0.0, 0.0);
    float hstep = 1.0/textureResolution.x;
    float vstep = 1.0/textureResolution.y;
    
    for(int i = 0; i < 7; i++)
    {
        vec2 uv = fragTextureCoord+vec2(hstep*(i-3)*dir.x, vstep*(i-3)*dir.y);
        outColor += texture(finalTexture, uv).xyz*blurArr[i>3?6-i:i];
    }

    finalColor = min(vec4(outColor, 1.0f), 1.0);
}