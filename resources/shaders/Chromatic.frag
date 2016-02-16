#version 420

in defaultBlock
{
	vec4 position;
	vec2 UV;
} inBlock;

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
	
	vec4 redValue = texture2D(defaultTexture, inBlock.UV - redOffset);
	vec4 greenValue = texture2D(defaultTexture, inBlock.UV - greenOffset);
	vec4 blueValue = texture2D(defaultTexture, inBlock.UV - blueOffset);

	outColor = vec4( redValue.r, greenValue.g, blueValue.b, 1.0f);
}