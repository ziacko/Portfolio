#version 440

in vec4 vPosition;
in vec2 vUV;

out vec4 outColor;

layout(std140, binding = 1) uniform chromaticSettings
{
	float redOffset;
	float greenOffset;
	float blueOffset;
};

uniform sampler2D defaultTexture;

void main()
{
	
	vec4 redValue = texture2D(defaultTexture, vUV - redOffset);
	vec4 greenValue = texture2D(defaultTexture, vUV - greenOffset);
	vec4 blueValue = texture2D(defaultTexture, vUV - blueOffset);

	outColor = vec4( redValue.r, greenValue.g, blueValue.b, 1.0f);
}