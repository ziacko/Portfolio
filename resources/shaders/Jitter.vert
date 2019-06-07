#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 biTangent;
layout (location = 4) in vec2 uv;

out defaultBlock
{
	vec4 prePos;
	vec4 newPos;
	vec2 uv;
	vec2 jitter;
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
	float		framesPerSecond;
	uint		totalFrames;
};

layout(std140, binding = 1) uniform velocitySettings
{
	mat4		previousProjection;
	mat4		previousView;
	mat4 		previousTranslation;
	
	mat4 		currentView;
};

layout(binding = 4) uniform jitterSettings
{
	vec2 haltonSequence[128];
	float haltonScale;
	uint numSamples;
	uint enableDithering;
	float ditheringScale;
};

void main()
{
	float deltaWidth = 1.0 / resolution.x;
	float deltaHeight = 1.0 / resolution.y;

	uint index = totalFrames % numSamples;
	vec2 jitter = vec2(haltonSequence[index].x * deltaWidth, haltonSequence[index].y * deltaHeight);

	mat4 newProj = projection;
	vec4 outPos = projection * view * translation * position;
	if(haltonScale > 0)
	{
    	newProj[3][0] += jitter.x * haltonScale;
    	newProj[3][1] += jitter.y * haltonScale;
		outPos.x += jitter.x * haltonScale;
		outPos.y += jitter.y * haltonScale;
	}

	outBlock.jitter = jitter;
	//needs to be jittered. nothing else does
	gl_Position = newProj * view * translation * position; 
	outBlock.uv = uv;

	//these 2 should be in screen space
	outBlock.newPos = projection * view * translation * position;
	outBlock.prePos = projection * previousView * previousTranslation * position; 
}
