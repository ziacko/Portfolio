#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 biTangent;
layout (location = 4) in vec2 uv;

out defaultBlock
{
	vec4 		worldPosition;
	vec4 		screenPosition;
	vec4 		normal;
	vec2		uv;
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
	uint 		totalFrames;
};

void main()
{
	outBlock.worldPosition = position; // add PRS matrix
	outBlock.screenPosition = projection * view * translation * position;
	outBlock.uv = uv;
	outBlock.normal = normal;
	
	gl_Position = outBlock.screenPosition;
}
