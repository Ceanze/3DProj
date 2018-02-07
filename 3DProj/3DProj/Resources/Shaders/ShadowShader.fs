#version 330

out float finalColor;

void main()
{
    finalColor = gl_FragCoord.z;
}