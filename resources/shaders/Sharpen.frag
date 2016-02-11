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

layout(std140, binding = 1) uniform sharpenSettings
{
	float		kernel1;
	float		kernel2;
	float		kernel3;
	float		kernel4;
	float		kernel5;
	float		kernel6;
	float		kernel7;
	float		kernel8;
	float		kernel9;
};

out vec4 outColor;

uniform sampler2D defaultTexture;

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
		float deltaWidth = 1.0 / resolution.x;
		float deltaHeight = 1.0 / resolution.y;

		vec2 offsets[9] = {
		vec2(-deltaWidth, -deltaHeight),	vec2(0.0, -deltaHeight),	vec2(deltaWidth, -deltaHeight),
		vec2(-deltaWidth, 0.0),				vec2(0.0, 0.0),				vec2(deltaWidth, 0.0),
		vec2(-deltaWidth, deltaHeight),		vec2(0.0, deltaHeight),		vec2(deltaWidth, deltaHeight)
		};

		vec4 color = vec4(0);
		color += texture2D(defaultTexture, inBlock.UV + offsets[0]) * kernel1;
		color += texture2D(defaultTexture, inBlock.UV + offsets[1]) * kernel2;
		color += texture2D(defaultTexture, inBlock.UV + offsets[2]) * kernel3;
		color += texture2D(defaultTexture, inBlock.UV + offsets[3]) * kernel4;
		color += texture2D(defaultTexture, inBlock.UV + offsets[4]) * kernel5;
		color += texture2D(defaultTexture, inBlock.UV + offsets[5]) * kernel6;
		color += texture2D(defaultTexture, inBlock.UV + offsets[6]) * kernel7;
		color += texture2D(defaultTexture, inBlock.UV + offsets[7]) * kernel8;
		color += texture2D(defaultTexture, inBlock.UV + offsets[8]) * kernel9;
		outColor = color;
	} 

	else
	{
		outColor = texture2D(defaultTexture, inBlock.UV);
	}	
}