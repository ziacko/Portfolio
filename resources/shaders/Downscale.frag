#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

in lanczosBlock
{
    vec2 centerUV;
    vec2 LStep1UV;
    vec2 LStep2UV;
    vec2 LStep3UV;
    vec2 LStep4UV;
    vec2 RStep1UV;
    vec2 RStep2UV;
    vec2 RStep3UV;
    vec2 RStep4UV;
} lancBlock;

out vec4 outColor;

layout(binding = 2) uniform downscaleSettings
{
    float       txlWidthOffset;
    float       txlHeightOffset;
	int 		downsampleMode;
};

layout(std140, binding = 3) uniform lanczosSettings
{
	float magicValue1;
	float magicValue2;
	float magicValue3;
	float magicValue4;
	float magicValue5;
};

layout(binding = 0) uniform sampler2D defaultTexture;

vec4 lanczos()
{
	vec4 fragColor = texture2D(defaultTexture, inBlock.uv) * magicValue1;
	
	fragColor += texture2D(defaultTexture, lancBlock.LStep1UV) * magicValue2;
	fragColor += texture2D(defaultTexture, lancBlock.RStep1UV) * magicValue2;

	fragColor += texture2D(defaultTexture, lancBlock.LStep2UV) * magicValue3;
	fragColor += texture2D(defaultTexture, lancBlock.RStep2UV) * magicValue3;

	fragColor += texture2D(defaultTexture, lancBlock.LStep3UV) * magicValue4;
	fragColor += texture2D(defaultTexture, lancBlock.RStep3UV) * magicValue4;

	fragColor += texture2D(defaultTexture, lancBlock.LStep4UV) * magicValue5;
	fragColor += texture2D(defaultTexture, lancBlock.RStep4UV) * magicValue5;	

	return fragColor;
}

void main()
{

	vec4 result = vec4(1, 0, 0, 1);

	if(downsampleMode == 0)
	{
		result = texture2D(defaultTexture, inBlock.uv);
	}

	else if (downsampleMode == 1)
	{
		result = lanczos();
	}

	outColor = result;
}