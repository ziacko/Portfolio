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

layout(std140, binding = 1) uniform contrastSettings
{
	float		contrast;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
		outColor = (texture2D(defaultTexture, inBlock.uv) - 0.5) * contrast + 0.5;
	}

	else
	{
		outColor = texture2D(defaultTexture, inBlock.uv);
	}

	
}