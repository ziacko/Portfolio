#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outPosition;

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

layout(binding = 0) uniform sampler2D diffuse;

float offsetX[4];
float offsetY[4];


void main()
{
	vec4 col =  texture(diffuse, inBlock.uv);
	vec2 newPos = gl_FragCoord.xy / resolution;
	//vec4 depth = vec4(vec3(gl_FragCoord.z), 1.0);

	float deltaWidth = 1.0 / resolution.x;
	float deltaHeight = 1.0 / resolution.y;
	offsetX = float[4](0.25f, 0.75f, 0.5f, 0.0f);
	offsetY = float[4](0.0f, 0.25f, 0.75f, 0.5f);

	uint index = totalFrames % 4;
    float randomX = fract(dot(gl_FragCoord.xy, vec2(0.375, -0.125)) + (offsetX[index] * deltaWidth));
    //float randomY = fract(dot(newPos, vec2(-0.125,  0.375))) - (offsetY[0] / deltaHeight);

	float noise = fract(randomX + totalTime * 15.2333);

	if(col.a < noise)
	{
		discard;
	}

	if(col.a == 0)
	{
		discard;
	}

	else
	{
		//col.a = 1;
	}

	

	outColor = col;//
	outPosition = vec4(newPos, 0.0, 1.0);
}
