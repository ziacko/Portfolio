#version 440

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;

out defaultBlock
{
	vec4 position;
	vec2 uv;
} outBlock;

out lanczosBlock
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

layout(binding = 2) uniform downscaleSettings
{
    float       txlWidthOffset;
    float       txlHeightOffset;
	int 		downsampleMode;
};

void ComputeLanczos(vec2 inputUV)
{
	vec2 deltaRes = vec2(1.0 / resolution.x, 1.0 / resolution.y);

	//vec2 offset1 = vec2(deltaRes.x, deltaRes.y);
	//vec2 offset2 = vec2(deltaRes.x * 2, deltaRes.y * 2);
	//vec2 offset3 = vec2(deltaRes.x * 3, deltaRes.y * 3);
	//vec2 offset4 = vec2(deltaRes.x * 4, deltaRes.y * 4);

	vec2 offset1 = vec2(txlWidthOffset, txlHeightOffset);
	vec2 offset2 = vec2(txlWidthOffset * 2, txlHeightOffset * 2);
	vec2 offset3 = vec2(txlWidthOffset * 3, txlHeightOffset * 3);
	vec2 offset4 = vec2(txlWidthOffset * 4, txlHeightOffset * 4);

	lancBlock.centerUV = inputUV;
	lancBlock.LStep1UV = inputUV - offset1;
	lancBlock.LStep2UV = inputUV - offset2;
	lancBlock.LStep3UV = inputUV - offset3;
	lancBlock.LStep4UV = inputUV - offset4;
	
	lancBlock.RStep1UV = inputUV + offset1;
	lancBlock.RStep2UV = inputUV + offset2;
	lancBlock.RStep3UV = inputUV + offset3;
	lancBlock.RStep4UV = inputUV + offset4;
}

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = outBlock.position.xy * 0.5f + 0.5f;
	ComputeLanczos(outBlock.uv);
	gl_Position = outBlock.position;
}