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
	float subPixShift;// = 0.25;
    float vxOffset;
    float maxSpan; //8.0
    float reduceMul;//1.0/8.0
    float reduceMin;//1.0/128
};

void main()
{
	vec2 deltaRes = vec2(1.0 / resolution.x, 1.0 / resolution.y);

	outBlock.position = projection * view * translation * position;
	outBlock.uv = outBlock.position.xy * 0.5f + 0.5f;
	gl_Position = outBlock.position;

	outBlock.fxaaShift.xy = outBlock.uv;
	outBlock.fxaaShift.zw = outBlock.uv - (deltaRes * (0.5 + subPixShift));

}