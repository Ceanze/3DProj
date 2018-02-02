#version 330

in vec2 fragTextureCoord;

uniform sampler2D finalTexture;
uniform vec2 textureResolution;
uniform vec2 dir;

out vec4 finalColor;

#define SIZE 6
#define STANDARD_DEVIATION 1.5

float gaussian(float x)
{
    const float ss2 = 2.0*STANDARD_DEVIATION*STANDARD_DEVIATION; 
    return (1.0/sqrt(3.1415*ss2)) * exp(-(x*x)/ss2);
}

//const float blurArr[4] = float[4](0.002291, 0.023226, 0.092651, 0.146768);

void main()
{
    float blurArr[SIZE+1];
    float sum = 0.0;
    for(int i = -SIZE; i <= 0; i++)
    {
        blurArr[i+SIZE] = gaussian(i);
        sum += blurArr[i+SIZE]*(i==0?1:2);
    }
    vec3 outColor = vec3(0.0, 0.0, 0.0);
    float hstep = 1.0/textureResolution.x;
    float vstep = 1.0/textureResolution.y;
    
    const int lineSize = SIZE*2+1;
    for(int i = 0; i < lineSize; i++)
    {
        int halfSize = lineSize/2;
        vec2 uv = fragTextureCoord+vec2(hstep*(i-halfSize)*dir.x, vstep*(i-halfSize)*dir.y);
        outColor += texture(finalTexture, uv).xyz*(blurArr[i>halfSize?lineSize-1-i:i]/sum);
    }

    finalColor = min(vec4(outColor, 1.0f), 1.0);
}