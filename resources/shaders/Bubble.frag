#version 440

in vec4 vPosition;
in vec2 vUV;

out vec4 outColor;

uniform sampler2D defaultTexture;

void main()
{
	outColor = texture2D(defaultTexture, vUV);
}