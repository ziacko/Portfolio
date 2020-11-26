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

layout(binding = 2) uniform vertSettings
{
	int			offsets[5];
	float		weights[5];
};

out vec4 outColor;

uniform sampler2D defaultTexture;

float pixelOffsets[5] = float[](1, 2, 3, 4, 5);

//uniform float offset[5] = float[]( 0.0, 1.0, 2.0, 3.0, 4.0 );
//uniform float weights[5] = float[]( 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 );


vec4 Gaussian()
{
	float deltaX = 1.0 / resolution.x;
	float deltaY = 1.0 / resolution.y;
	
	vec4 sum = texture(defaultTexture, inBlock.uv) * weights[0];

	for(uint iter = 0; iter < 5; iter++)
	{
		sum += texture(defaultTexture, (inBlock.uv + vec2(0.0, offsets[iter]) / resolution.y )) * weights[iter];
		sum += texture(defaultTexture, (inBlock.uv - vec2(0.0, offsets[iter]) / resolution.y )) * weights[iter];
	}

	return sum;
}

void main()
{
	outColor = Gaussian();
}