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

layout(std140, binding = 1) uniform sepiaSettings
{
	float		factor;
	float		redModifier1;
	float		redModifier2;
	float		redModifier3;

	float		greenModifier1;
	float		greenModifier2;
	float		greenModifier3;

	float		blueModifier1;
	float		blueModifier2;
	float		blueModifier3;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

vec4 Sepia( in vec4 color )
{
	return vec4(
			clamp(color.r * redModifier1 + color.g * greenModifier1 + color.b * blueModifier1, 0.0, 1.0),
			clamp(color.r * redModifier2 + color.g * greenModifier2 + color.b * blueModifier2, 0.0, 1.0),
			clamp(color.r * redModifier3 + color.g * greenModifier3 + color.b * blueModifier3, 0.0, 1.0),
			color.a);
}

void main()
{
	vec4 texColor = texture2D(defaultTexture, inBlock.UV);

	if(gl_FragCoord.x < mousePosition.x)
	{
		outColor = texColor;
	}

	else
	{
		outColor = mix(texColor, Sepia(texColor), clamp(factor, 0.0, 1.0));
	}
}