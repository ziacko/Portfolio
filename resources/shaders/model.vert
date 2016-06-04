#version 420

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UV;

out defaultBlock
{
	vec4 		position;
	vec2		UV;
} outBlock;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.UV = UV;
	
	gl_Position = outBlock.position;
}
