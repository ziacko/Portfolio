#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;

out defaultBlock
{
	vec4 position;
	vec2 uv;
} outBlock;

out edgeBlock
{
    vec4 offset[3];
} outEdge;

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
 * Edge Detection Vertex Shader
 */
void SMAAEdgeDetectionVS(vec2 texcoord, out vec4 offset[3]) 
{
    offset[0] = fma(deltaResolution.xyxy, vec4(-1.0, 0.0, 0.0, -1.0), texcoord.xyxy);
    offset[1] = fma(deltaResolution.xyxy, vec4( 1.0, 0.0, 0.0,  1.0), texcoord.xyxy);
    offset[2] = fma(deltaResolution.xyxy, vec4(-2.0, 0.0, 0.0, -2.0), texcoord.xyxy);
}

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = outBlock.position.xy * 0.5f + 0.5f;
    SMAAEdgeDetectionVS(outBlock.uv, outEdge.offset);

	gl_Position = outBlock.position;
}