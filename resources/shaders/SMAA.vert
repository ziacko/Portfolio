#version 440
#define SMAA_MAX_SEARCH_STEPS 32

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;

out defaultBlock
{
	vec4 position;
	vec2 uv;
} outBlock;

out blendBlock
{
	vec4 offset;
} outBlend;

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

vec2 deltaResolution = vec2(1.0 / resolution.x, 1.0 / resolution.y );

/**
 * Neighborhood Blending Vertex Shader
 */
void SMAANeighborhoodBlendingVS(vec2 texcoord, out vec4 offset)
{
    offset = fma(deltaResolution.xyxy, vec4( 1.0, 0.0, 0.0, 1.0), texcoord.xyxy);
}

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = outBlock.position.xy * 0.5f + 0.5f;
	SMAANeighborhoodBlendingVS(outBlock.uv, outBlend.offset);
	gl_Position = outBlock.position;
}