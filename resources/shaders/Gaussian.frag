#version 440

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

layout(std140, binding = 1) uniform gaussianSettings
{
	uint		numOffsets;
	float		weight;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

float pixelOffsets[5] = float[](1, 2, 3, 4, 5);

vec4 Gaussian()
{
	float deltaX = 1.0 / resolution.x;
	float deltaY = 1.0 / resolution.y;
	
	vec4 sum = texture(defaultTexture, inBlock.UV) * weight;

	for	(int iter = 0; iter < 5; iter++)
	{
		sum += texture(defaultTexture, inBlock.UV + vec2(pixelOffsets[iter]) / resolution.x, 0.0f) * weight;
		sum += texture(defaultTexture, inBlock.UV - vec2(pixelOffsets[iter]) / resolution.x, 0.0f) * weight;

		//sum += texture(defaultTexture, inBlock.UV + vec2(0.0, pixelOffsets[iter]) * deltaY) * weight;
		//sum += texture(defaultTexture, inBlock.UV - vec2(0.0, pixelOffsets[iter]) * deltaY) * weight;
	}

	return sum;
}

void main()
{
	outColor = Gaussian();
}