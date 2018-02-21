#version 330

in vec2 fragTextureCoord;

uniform sampler2D finalTexture;
uniform vec2 textureResolution;
uniform vec2 dir;

out vec4 finalColor;

// 9x9 kernal.
const float blurArr[5] = {0.00759732401586, 0.0359939776755, 0.109340049784, 0.212965337015, 0.265961520268};
const float sum = 0.997754897248;
const int lineSize = 9;
const int halfSize = 4;

void main()
{
    vec3 outColor = vec3(0.0, 0.0, 0.0);
    float hstep = 1.0/textureResolution.x;
    float vstep = 1.0/textureResolution.y;
    
    for(int i = 0; i < lineSize; i++)
    {
        vec2 uv = fragTextureCoord+vec2(hstep*(i-halfSize)*dir.x, vstep*(i-halfSize)*dir.y);
        outColor += texture(finalTexture, uv).xyz * (blurArr[i>halfSize?lineSize-1-i:i]/sum);
    }

    finalColor = min(vec4(outColor, 1.0f), 1.0);
}