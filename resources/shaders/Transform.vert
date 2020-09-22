#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 biTangent;
layout (location = 4) in vec2 uv;

out defaultBlock
{
	vec4 		position;
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

layout(std140, binding = 1) uniform transformSettings
{
    mat4 modelPosition;
    mat4 modelRotation;
    mat4 modelScale;
    vec4 modelEulerAngles;
};

void main()
{
    vec4 modelPRS = modelPosition * modelRotation * modelScale * position;

	outBlock.position = projection * view * translation * modelPRS;
	outBlock.uv = uv;
	
	gl_Position = outBlock.position;
}
