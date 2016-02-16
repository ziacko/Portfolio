#version 420

in defaultBlock
{
	vec4 position;
	vec2 UV;
} inBlock;

out vec4 outColor;

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

layout(std140, binding = 1) uniform gammaSettings
{
	vec4		gamma;
};

uniform sampler2D defaultTexture;

void main()
{
	if(gl_FragCoord.x > mousePosition.x)
	{
		outColor = texture2D(defaultTexture, inBlock.UV);
	}

	else
	{
		outColor = pow( texture2D(defaultTexture, inBlock.UV), 1.0 / gamma);
	}
}