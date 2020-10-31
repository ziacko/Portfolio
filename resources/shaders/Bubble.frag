#version 440
#extension GL_NV_gpu_shader_fp64 : enable

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
	//ok puck into an unsinged int? now what
	//vec2 inOffset = unpackHalf2x16(texture(perlin, vec2(inBlock.uv.x, 1 - inBlock.uv.y)).r);

    float perlinLevel = length(texture(perlin, vec2(inBlock.uv.x, 1 - inBlock.uv.y)).rg);
	if(perlinLevel > 0.0f)
    {
		vec2 pushVector = normalize(dispPosition) * perlinLevel * offset;
		dispPosition += pushVector;
		//should already be scaled
        dispUV = dispPosition * 0.5f + 0.5f;
		dispUV += perlinLevel * offset;
	}
	vec2 test = vec2(dispUV);
	outColor = texture(defaultTexture, test);
}