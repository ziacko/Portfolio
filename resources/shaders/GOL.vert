#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 UV;

out defaultBlock
{
	vec4 position;
	vec2 uv;
	vec4 color;
} outBlock;

//unsigned int NeighborCount = 0;
//unsigned int DeadNeighborCount = 0;

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
//ok use a shader storage buffer to store all tihs cell data
layout(std140, binding = 1) uniform GOLSettings 
{
	vec4 AliveColor;
	vec4 DeadColor;
	vec4 EmptyColor;
	float Dimensions;
};

layout(binding = 0) buffer GOLStatus 
{
	int Status[];
};

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = UV;// / Resolution;

	//oooooh right this uses instanced rendering.
	float XResult = 0;
	float YResult = modf(gl_InstanceID / Dimensions, XResult); 
	outBlock.position.x = outBlock.position.x + (XResult * ((resolution.x / Dimensions) / resolution.x)) * 2;
	outBlock.position.y = outBlock.position.y - YResult * 2;

	if(Status[gl_InstanceID] == 0)
	{
		outBlock.color = vec4(0, 0, 1, 1);
	}

	if(Status[gl_InstanceID] == 1)
	{
		outBlock.color = vec4(0, 1, 0, 1);
	}

	if(Status[gl_InstanceID] == 2)
	{
		outBlock.color = vec4(1, 0, 0, 1);
	}

	gl_Position = outBlock.position;
}