#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

out vec4 outColor;

layout(binding = 0) uniform sampler2D defaultTexture;

void main()
{
	outColor = texture2D(defaultTexture, inBlock.uv);
}