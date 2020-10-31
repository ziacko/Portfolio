#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
	
} inBlock;

layout(location = 0) out vec4 outColor;

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

layout(binding = 1) uniform godRaySettings
{
    vec4 lightPosition;
    float decay;
    float density;
    float weight;
    float exposure;
	int samples;
};

layout(binding = 1) uniform sampler2D occlusion;
layout(binding = 0) uniform sampler2D color;

vec2 WorldToScreen(in vec4 inPosition, vec2 resolution)
{
	//surely this is all you'd need for a world to screen transform
	inPosition = view * projection * inPosition;

	//what is this part now?
	float deltaWidth = round(((inPosition.x + 1) / 2) * resolution.x);
	float deltaHeight = round(((1 - inPosition.y) / 2) * resolution.y);
	//i think this is a bit sus (might replace later)
	return vec2(deltaWidth, deltaHeight);
}

vec2 WorldToScreen2()
{
	//surely this is all you'd need for a world to screen transform
	vec4 mousePos = view * projection * vec4(mousePosition, 0, 1);

	//what is this part now?
	float deltaWidth = round((mousePos.x / 2) * resolution.x);
	float deltaHeight = round((mousePos.y / 2) * resolution.y);
	//i think this is a bit sus (might replace later)
	return round((mousePos.xy / 2) * resolution);
}

void main()
{
	vec4 colorSample = texture(color, inBlock.uv);
	vec2 deltaUV = vec2(inBlock.uv - WorldToScreen2());
	deltaUV *= 1.0 / float(samples) * density;

	vec2 tempUV = inBlock.uv;
	float illuminationDecay = 1;

	float occlusionSample2 = texture(occlusion, tempUV).r;
	if(occlusionSample2 < 1) //should be 1 or 0 but since it's a ubyte....
	{
		//a for loop this big looks REALLY bad for performance
		for(int iter = 0; iter < samples; iter++)
		{
			tempUV -= deltaUV;
			float occlusionSample = texture(occlusion, tempUV).r;
			occlusionSample *= illuminationDecay * weight;

			colorSample += occlusionSample;
			illuminationDecay *= decay;
		}
		colorSample *= exposure;
	}	
	outColor = colorSample;
}