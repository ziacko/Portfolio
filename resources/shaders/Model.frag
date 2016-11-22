#version 420

in defaultBlock
{
	vec4 		position;
	vec2		UV;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4 		view;
	mat4 		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSec;
};

void main()
{
	outColor = vec4(1, 0, 0, 1);
}