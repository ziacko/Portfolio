#version 420

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4 		view;
	mat4 		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint 		totalFrames;
};

layout(std140, binding = 1) uniform SBHQSettings
{
	float scale;
};


uniform sampler2D defaultTexture;

//superbible High quality filter
vec4 SBHQFilter()
{
    vec2 texSize = textureSize(defaultTexture, 0);

    vec2 uvScaled = inBlock.uv * texSize + scale;

    vec2 uvInt = floor(uvScaled);
    vec2 uvFrac = fract(uvScaled);

    uvFrac = smoothstep(0.0, 1.0, uvFrac);

    vec2 finalUV = (uvInt + uvFrac - scale) / texSize;
    return texture(defaultTexture, finalUV);
}

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
        outColor = SBHQFilter();
	} 

	else
	{
		outColor = texture2D(defaultTexture, inBlock.uv);
	}	
}