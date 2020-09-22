#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

out vec4 outColor;

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

layout (std140, binding = 2) uniform bubbleSettings
{
	float attenuation;
	float offset;
	float dimensions;
};

layout(binding = 0) uniform sampler2D defaultTexture;
layout(binding = 1) uniform sampler2D perlin;

void main()
{
	vec2 dispUV = inBlock.uv;
	vec2 dispPosition = inBlock.position.xy;
	//slip the image UV
    uint perlinLevel = packHalf2x16(texture2D(perlin, vec2(inBlock.uv.x, 1 - inBlock.uv.y)).rg);
	if(perlinLevel > 0.0f)
    {
		vec2 pushVector = normalize(dispPosition) * perlinLevel * offset;
		dispPosition += pushVector;
		//should already be scaled
        dispUV = dispPosition * 0.5f + 0.5f;
		dispUV += perlinLevel * offset;
	}

	outColor = texture2D(defaultTexture, dispUV);
}