#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

out vec4 outColor;

layout(binding = 0) uniform sampler2D vertical;
layout(binding = 1) uniform sampler2D horizontal;

void main()
{
	outColor = mix(texture(vertical, inBlock.uv), texture(horizontal, inBlock.uv), 0.5);
}