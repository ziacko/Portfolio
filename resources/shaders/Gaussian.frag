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
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint		totalFrames;
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
	vec2 delta = vec2(gl_FragCoord.xy / resolution);
	
	vec4 sum = texture2D(defaultTexture, delta) * weight1;

	sum += texture2D(defaultTexture, delta + (vec2(offset1, offset1) / resolution )) * weight1;
	sum += texture2D(defaultTexture, delta - (vec2(offset1, offset1) / resolution )) * weight1;

	sum += texture2D(defaultTexture, delta + (vec2(offset2, offset2) / resolution )) * weight2;
	sum += texture2D(defaultTexture, delta - (vec2(offset2, offset2) / resolution )) * weight2;

	sum += texture2D(defaultTexture, delta + (vec2(offset3, offset3) / resolution )) * weight3;
	sum += texture2D(defaultTexture, delta - (vec2(offset3, offset3) / resolution )) * weight3;

	sum += texture2D(defaultTexture, delta + (vec2(offset4, offset4) / resolution )) * weight4;
	sum += texture2D(defaultTexture, delta - (vec2(offset4, offset4) / resolution )) * weight4;

	sum += texture2D(defaultTexture, delta + (vec2(offset5, offset5) / resolution )) * weight5;
	sum += texture2D(defaultTexture, delta - (vec2(offset5, offset5) / resolution )) * weight5;

	return sum;
}

void main()
{
	outColor = Gaussian();
}