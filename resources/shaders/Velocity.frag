#version 440

in defaultBlock
{
	vec4 prePos;
	vec4 newPos;
	vec2 uv;
	vec2 jitter;
} inBlock;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outVelocity;

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

layout(binding = 0) uniform sampler2D diffuse;

float OffsetX[4]; //use quincux set here. couldn't get halton to work here
float OffsetY[4];

void main()
{
	//clamp the alpha down hard. if alpha is less than 0.1, clamp it to 0

	//move this to UV space
	vec2 newPos = ((inBlock.newPos.xy / inBlock.newPos.w) * 0.5 + 0.5);
	vec2 prePos = ((inBlock.prePos.xy / inBlock.prePos.w) * 0.5 + 0.5);

	float deltaWidth = 1.0 / resolution.x;
	float deltaHeight = 1.0 / resolution.y;
	OffsetX = float[4](0.25f, 0.75f, 0.5f, 0.0f);
	OffsetY = float[4](0.0f, 0.25f, 0.75f, 0.5f);

	uint index = totalFrames % 4;
    float randomX = fract(dot(gl_FragCoord.xy, vec2(0.375, -0.125)) + (OffsetY[index] * deltaWidth));
    float randomY = fract(dot(vec2(-0.125,  0.375), gl_FragCoord.xy)) - (OffsetX[index] / deltaHeight);
	float noise = fract(fract(randomX + randomY) + totalTime * ditheringScale);

	vec2 velocity = (newPos - prePos);// * 10;
	//velocity = clamp(velocity, vec2(0), vec2(maxVelocity)) * 10;

	vec4 col = texture(diffuse, inBlock.uv);

	if(col.a < noise && enableDithering > 0)
	{
		discard;
	}

	if(col.a < 0.5)
	{
		discard;
	}

	else
	{
		col.a = 1;
	}

	outVelocity = vec4(velocity, 0, 1);
	outColor = col;
}