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

layout(std140, binding = 1) uniform velocitySettings
{
	mat4		previousProjection;
	mat4		previousView;
	mat4 		previousTranslation;
};

layout(binding = 4) uniform jitterSettings
{
	vec2 haltonSequence[128];
	float haltonScale;
	uint numSamples;
	uint offsetX;
	uint offsetY;
};

void main()
{
	float deltaWidth = 1.0 / resolution.x;
	float deltaHeight = 1.0 / resolution.y;

	//outBlock.position = projection * view * translation * position;	
	//outBlock.uv = uv;// = outBlock.position.xy * 0.5f + 0.5f;

	/*uint index = (totalFrames -1) % numSamples;
    //float randomX = fract(dot(position.xy, vec2(0.375, -0.125))) + (haltonSequence[index].x * deltaWidth);
    //float randomY = fract(dot(position.xy, vec2(-0.125,  0.375))) + (haltonSequence[index].y * deltaHeight);
    //uint index = totalFrames % numSamples;
    float randomX = (haltonSequence[index].x * deltaWidth)  * haltonScale;
    float randomY = (haltonSequence[index].y * deltaHeight) * haltonScale;

    mat4 newProj = projection;
    newProj[3][0] += randomX * 2.0f - 1.0f;
    newProj[3][1] += randomY * 2.0f - 1.0f;*/

	uint index = (totalFrames - 1) % numSamples;
	float randomX = haltonSequence[index].x * deltaWidth;
	float randomY = haltonSequence[index].y * deltaHeight;

	mat4 newProj = projection;
    newProj[3][0] += randomX / offsetX;
    newProj[3][1] += randomY / offsetY;

	outBlock.position = newProj * previousView * previousTranslation * position;
	outBlock.uv = uv;
	gl_Position = outBlock.position;
}
