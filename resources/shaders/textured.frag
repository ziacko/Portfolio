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
	outColor = texture2D(defaultTexture, vec2(inBlock.UV.x,1.0 - inBlock.UV.y));
}