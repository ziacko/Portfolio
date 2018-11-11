#version 420

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

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

layout(std140, binding = 3) uniform sharpenSettings
{
	float		kernel1;
	float		kernel2;
};

out vec4 outColor;

layout(binding = 0) uniform sampler2D defaultTexture;

void main()
{

	float deltaWidth = 1.0 / resolution.x;
	float deltaHeight = 1.0 / resolution.y;

	vec4 sum = vec4(0.0);
  	sum += kernel1 * texture2D(defaultTexture, inBlock.uv + vec2( -1.0 * deltaWidth , 0.0 * deltaHeight));
  	sum += kernel1 * texture2D(defaultTexture, inBlock.uv + vec2( 0.0 * deltaWidth , -1.0 * deltaHeight));
  	sum += kernel2 * texture2D(defaultTexture, inBlock.uv + vec2( 0.0 * deltaWidth , 0.0 * deltaHeight));
  	sum += kernel1 * texture2D(defaultTexture, inBlock.uv + vec2( 0.0 * deltaWidth , 1.0 * deltaHeight));
  	sum += kernel1 * texture2D(defaultTexture, inBlock.uv + vec2( 1.0 * deltaWidth , 0.0 * deltaHeight));
	outColor = sum;
}