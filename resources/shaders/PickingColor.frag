#version 450

in defaultBlock
{
	vec4 		position;
	vec4 		normal;
	vec2		uv;
} inBlock;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outPick;

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

layout(std140, binding = 1) uniform pickSettings
{
    vec4        color;
};

layout(binding = 0) uniform sampler2D diffuse;

void main()
{
	//clamp the alpha down hard. if alpha is less than 0.1, clamp it to 0
	vec4 col =  texture(diffuse, inBlock.uv);

	if(col.a < 0.5)
	{
		discard;
	}

	else
	{
		col.a = 1;
	}

	outColor = col;
}