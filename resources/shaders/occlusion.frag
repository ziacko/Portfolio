#version 440

in defaultBlock
{
	vec4 position;
    vec4 normal;
	vec2 uv;
	
} inBlock;

out float outColor;

void main()
{
	outColor = 1;
}