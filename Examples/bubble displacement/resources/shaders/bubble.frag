#version 440

in vec4 vPosition;
in vec2 vUV;

out vec4 outColor;

uniform sampler2D bubbleTexture;

void main()
{
	outColor = texture2D(bubbleTexture, vUV);
	//outColor = vec4(1, 0, 0, 1);
}