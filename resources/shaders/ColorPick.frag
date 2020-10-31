#version 450

in defaultBlock
{
	vec4 		position;
	vec4 		normal;
	vec2		uv;
} inBlock;

layout(location = 0) out float outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4 		view;
	mat4 		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint		totalFrames;
};

layout(binding = 2) uniform pickColors
{
	vec4		color;
};

layout(binding = 0) uniform sampler2D diffuse;

void main()
{
	outColor = color.r;//texture(diffuse, inBlock.uv);
}