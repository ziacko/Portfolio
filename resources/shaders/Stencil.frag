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

layout(binding = 0) uniform sampler2D defaultTexture;
layout(binding = 1) uniform usampler2D stencilTexture;

void main()
{
    uint stencil = texture(stencilTexture, inBlock.uv).r;
	vec4 defaultColor = texture2D(defaultTexture, inBlock.uv);

	if(gl_FragCoord.x < mousePosition.x)
	{
        if(stencil > 0)
        {
            outColor = vec4(0, 1, 0, 1);
        }
        else
        {
            outColor = vec4(1, 0, 0, 1);
        }
	}

	else
	{
		outColor = defaultColor;
	}
}