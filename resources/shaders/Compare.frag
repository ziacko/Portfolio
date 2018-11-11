#version 440

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

out vec4 outColor;

layout(binding = 0) uniform sampler2D compareTexture;
layout(binding = 1) uniform sampler2D defaultTexture;

void main()
{
	vec4 defaultColor = texture2D(defaultTexture, inBlock.uv);
	vec4 compareColor = texture2D(compareTexture, inBlock.uv);

	if(gl_FragCoord.x < mousePosition.x)
	{
		outColor = defaultColor;
	}

	else
	{
		outColor = compareColor;
	}
}