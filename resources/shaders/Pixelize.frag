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

layout(std140, binding = 1) uniform pixelizeSettings
{
	float		pixelWidth;
	float		pixelHeight;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

void main()
{
	float deltaX = pixelWidth * (1.0 / resolution.x);
	float deltaY = pixelHeight * (1.0 / resolution.y);

	vec2 uv = vec2(deltaX * floor(inBlock.UV.x / deltaX),
	deltaY * floor(inBlock.UV.y / deltaY));



	outColor = texture2D(defaultTexture, uv);

	
}