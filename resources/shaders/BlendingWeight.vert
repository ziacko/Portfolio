#version 440
#define SMAA_MAX_SEARCH_STEPS 112

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;

out defaultBlock
{
	vec4 position;
	vec2 uv;
} outBlock;

out blendBlock
{
	vec4 offset[3];
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
 * Blend Weight Calculation Vertex Shader
 */
void SMAABlendingWeightCalculationVS(vec2 texcoord, out vec4 offset[3]) 
{
    //pixcoord = texcoord * resolution;

    // We will use these offsets for the searches later on (see @PSEUDO_GATHER4):
    offset[0] = fma(deltaResolution.xyxy, vec4(-0.25, -0.125,  1.25, -0.125), texcoord.xyxy);
    offset[1] = fma(deltaResolution.xyxy, vec4(-0.125, -0.25, -0.125,  1.25), texcoord.xyxy);

    // And these for the searches, they indicate the ends of the loops:
    offset[2] = fma(deltaResolution.xxyy, vec4(-2.0, 2.0, -2.0, 2.0) * float(SMAA_MAX_SEARCH_STEPS),
                    vec4(offset[0].xz, offset[1].yw));
}

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = outBlock.position.xy * 0.5f + 0.5f;
	SMAABlendingWeightCalculationVS(outBlock.uv, outBlend.offset);
	gl_Position = outBlock.position;
}