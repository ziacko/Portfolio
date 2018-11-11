#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;

out defaultBlock
{
	vec4 position;
	vec4 fxaaShift;
	vec2 uv;
} outBlock;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint		totalFrames;
};

layout(std140, binding = 5) uniform fxaaSettings
{
	float subPixshift;// = 0.25;
};

void main()
{
	vec2 deltaRes = vec2(1.0 / resolution.x, 1.0 / resolution.y);

	outBlock.position = projection * view * translation * position;
	outBlock.uv = outBlock.position.xy * 0.5f + 0.5f;
	gl_Position = outBlock.position;

	outBlock.fxaaShift.xy = outBlock.xy;
	outBlock.fxaaShift.zw = outBlock.xy - (deltaRes * (0.5 + subPixshift));

}