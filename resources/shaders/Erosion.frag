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

layout(std140, binding = 1) uniform erosionSettings
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
		vec4 minValue = vec4(1.0);

		float deltaWidth = 1.0 / resolution.x * strengthX;
		float deltaHeight = 1.0 / resolution.y * strengthY;

		vec2 offsets[9] = {
		vec2(-deltaWidth, -deltaHeight),	vec2(0.0, -deltaHeight),	vec2(deltaWidth, -deltaHeight),
		vec2(-deltaWidth, 0.0),				vec2(0.0, 0.0),				vec2(deltaWidth, 0.0),
		vec2(-deltaWidth, deltaHeight),		vec2(0.0, deltaHeight),		vec2(deltaWidth, deltaHeight)
		};

		for(int iter = 0; iter < 9; iter++)
		{
			minValue = min(texture2D(defaultTexture, inBlock.UV + offsets[iter]), minValue);
		}

		outColor = minValue;
	} 

	else
	{
		outColor = texture2D(defaultTexture, inBlock.UV);
	}	
}