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
	int			offset1;
	int			offset2;
	int			offset3;
	int			offset4;
	int			offset5;
	
	float		weight1;
	float		weight2;
	float		weight3;
	float		weight4;
	float		weight5;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

float pixelOffsets[5] = float[](1, 2, 3, 4, 5);

uniform float offset[5] = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );
uniform float weights[5] = float[]( 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 );


vec4 Gaussian()
{
	float deltaX = 1.0 / resolution.x;
	float deltaY = 1.0 / resolution.y;
	
	vec4 sum = texture(defaultTexture, inBlock.UV) * weight1;

	sum += texture(defaultTexture, (inBlock.UV + vec2(0.0f, offset1)) / resolution.x ) * weight1;
	sum += texture(defaultTexture, (inBlock.UV - vec2(0.0f, offset1)) / resolution.y ) * weight1;

	sum += texture(defaultTexture, (inBlock.UV + vec2(0.0f, offset2)) / resolution.x ) * weight2;
	sum += texture(defaultTexture, (inBlock.UV - vec2(0.0f, offset2)) / resolution.y ) * weight2;

	sum += texture(defaultTexture, (inBlock.UV + vec2(0.0f, offset3)) / resolution.x ) * weight3;
	sum += texture(defaultTexture, (inBlock.UV - vec2(0.0f, offset3)) / resolution.y ) * weight3;

	sum += texture(defaultTexture, (inBlock.UV + vec2(0.0f, offset4)) / resolution.x ) * weight4;
	sum += texture(defaultTexture, (inBlock.UV - vec2(0.0f, offset4)) / resolution.y ) * weight4;

	sum += texture(defaultTexture, (inBlock.UV + vec2(0.0f, offset5)) / resolution.x ) * weight5;
	sum += texture(defaultTexture, (inBlock.UV - vec2(0.0f, offset5)) / resolution.y ) * weight5;

	return sum;
}

void main()
{
	outColor = Gaussian();
}