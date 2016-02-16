#version 420

in defaultBlock
{
	vec4 position;
	vec2 UV;
} inBlock;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
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
		outColor = (texture2D(defaultTexture, inBlock.UV) - 0.5) * contrast + 0.5;
	}

	else
	{
		outColor = texture2D(defaultTexture, inBlock.UV);
	}

	
}