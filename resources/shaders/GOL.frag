#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
	vec4 color;
	
} inBlock;

out vec4 outColor;

void main()
{
	outColor = inBlock.color;
}