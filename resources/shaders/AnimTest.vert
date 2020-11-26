#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 biTangent;
layout (location = 4) in uint boneIndex;
layout (location = 5) in float weight;
layout (location = 6) in vec2 uv;

out defaultBlock
{
	vec4 		position;
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

layout(binding = 0) buffer boneSettings
{
    mat4 Bones[];
};

void main()
{
	//move from world space to screen space
	mat4 mvp = projection * view * translation;
	vec4 pos = Bones[boneIndex] * position * weight;
	outBlock.position = projection * view * translation * pos;// Bones[boneIndex] * position * weight;

	outBlock.uv = uv;
	outBlock.normal = normal;
	
	gl_Position = outBlock.position;
}
