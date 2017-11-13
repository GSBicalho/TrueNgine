#version 330 core

uniform vec4 inColor;

out vec4 FragColor;
in vec4 colorModifier;

void main()
{
    FragColor.xyz = normalize(colorModifier.xyz);
	FragColor.w = 0.2f;
}