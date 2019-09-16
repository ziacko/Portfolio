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

layout(std140, binding = 1) uniform dilationSettings
{
	float		strengthX;
	float		strengthY;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
		vec4 samples[9];
		vec4 maxValue = vec4(0.0);

		float deltaWidth = 1.0 / resolution.x * strengthX;
		float deltaHeight = 1.0 / resolution.y * strengthY;

		vec2 offsets[9] = {
		vec2(-deltaWidth, -deltaHeight),	vec2(0.0, -deltaHeight),	vec2(deltaWidth, -deltaHeight),
		vec2(-deltaWidth, 0.0),				vec2(0.0, 0.0),				vec2(deltaWidth, 0.0),
		vec2(-deltaWidth, deltaHeight),		vec2(0.0, deltaHeight),		vec2(deltaWidth, deltaHeight)
		};

		for(int iter = 0; iter < 9; iter++)
		{
			maxValue = max(texture2D(defaultTexture, inBlock.uv + offsets[iter]), maxValue);
		}

		outColor = maxValue;
	} 

	else
	{
		outColor = texture2D(defaultTexture, inBlock.uv);
	}
}