#version 450

in defaultBlock
{
	vec4 	position;
	vec2 	uv;
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

layout(std140, binding = 1) uniform outlineSettings
{
	float uvScale;
};

layout(binding = 0) uniform sampler2D defaultTexture;
layout(binding = 1) uniform usampler2D stencilTexture;

void main()
{

	vec2 deltaResolution = 1.0 / resolution;

    uint defaultStencil = texture(stencilTexture, inBlock.uv).r;
  //  vec2 zoomedUV = clamp(inBlock.uv * uvScale, inBlock.uv, vec2(1));
   // uint zoomedStencil = texture(stencilTexture, zoomedUV).r;

	vec4 defaultColor = texture2D(defaultTexture, inBlock.uv);

	uint leftStencil = texture(stencilTexture, inBlock.uv + vec2(-deltaResolution.x, 0.0f)).r;
	uint rightStencil = texture(stencilTexture, inBlock.uv + vec2(deltaResolution.x, 0.0f)).r;
	uint upStencil = texture(stencilTexture, inBlock.uv + vec2(0.0f, -deltaResolution.y)).r;
	uint downStencil = texture(stencilTexture, inBlock.uv + vec2(0.0f, deltaResolution.y)).r;

	bool shouldReplace = false;
	//ok let's do a 2x2 kernel for the stencil buffer
	//if any of the texels in the kernel do not match the current stencil value at this texel,
	//replace color with stencil mask color
	if(leftStencil != defaultStencil)
	{
		shouldReplace = true;
	}

	if(rightStencil != defaultStencil)
	{
		shouldReplace = true;
	}

	if(upStencil != defaultStencil)
	{
		shouldReplace = true;
	}

	if(downStencil != defaultStencil)
	{
		shouldReplace = true;
	}

	if(inBlock.uv.y == 0 || inBlock.uv.y == 1)
	{
		shouldReplace = false;
	}

	outColor = defaultColor;
	if(shouldReplace)
	{
		outColor = vec4(0, 1, 0, 1);
	}
	else
	{
		outColor = defaultColor;//vec4(1, 0, 0, 1);
	}
}