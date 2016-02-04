#version 440

in defaultBlock
{
	vec4 position;
	vec2 UV;
} inBlock;

out vec4 outColor;

uniform sampler2D defaultTexture;

void main()
{
	outColor = texture2D(defaultTexture, inBlock.UV);
}