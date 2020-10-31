#version 420

in defaultBlock
{
	vec4 		position;
	vec4 		normal;
	vec2		uv;
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
	float 		framesPerSecond;
	uint 		totalFrames;
};

layout(std140, binding = 1) uniform materialSettings
{
	vec4 		diffuseMat;
	vec4		specularMat;
	vec4		ambientMat;
	vec4		emissiveMat;
};

uniform sampler2D diffuse;

void main()
{
	outColor = texture2D(diffuse, inBlock.uv);
}