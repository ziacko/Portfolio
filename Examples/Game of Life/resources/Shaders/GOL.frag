#version 440
in vec4 vPosition;
in vec2 vUV;
in vec4 vColor;

out vec4 outColor;

void main()
{
	outColor = vColor;
}