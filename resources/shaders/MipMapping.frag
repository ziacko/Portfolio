#version 440
#extension GL_ARB_texture_query_lod : enable
in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

layout(location = 0) out vec4 outColor;

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

layout(std140, binding = 1) uniform mipSettings
{
	int		mipLevel;
};

layout(binding = 0) uniform sampler2D defaultTexture;

void main()
{
	float mipMapLevel = textureQueryLod(defaultTexture, inBlock.uv).x;
	outColor = textureLod(defaultTexture, inBlock.uv, mipLevel);
}