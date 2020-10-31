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

layout(std140, binding = 2) uniform integerSettings
{
	vec2 inSize;
    vec2 outSize;

};

uniform sampler2D defaultTexture;

vec4 blTexture(in vec2 inUV, in vec2 texSize)
{
	// subtract 0.5 here and add it again after the floor to centre the texel
	vec2 texCoord = inUV * texSize - vec2(0.5);
	vec2 s0t0 = floor(texCoord) + vec2(0.5);
	vec2 s0t1 = s0t0 + vec2(0.0, 1.0);
	vec2 s1t0 = s0t0 + vec2(1.0, 0.0);
	vec2 s1t1 = s0t0 + vec2(1.0);

	vec2 invTexSize = 1.0 / texSize;
	vec4 c_s0t0 = texture(defaultTexture, s0t0 * invTexSize);
	vec4 c_s0t1 = texture(defaultTexture, s0t1 * invTexSize);
	vec4 c_s1t0 = texture(defaultTexture, s1t0 * invTexSize);
	vec4 c_s1t1 = texture(defaultTexture, s1t1 * invTexSize);

	vec2 weight = fract(texCoord);

	vec4 c0 = c_s0t0 + (c_s1t0 - c_s0t0) * weight.x;
	vec4 c1 = c_s0t1 + (c_s1t1 - c_s0t1) * weight.x;

	return (c0 + (c1 - c0) * weight.y);
}

vec4 nnTexture(in vec2 inUV, in vec2 texSize)
{
	
	vec2 texCoord = floor(inUV * texSize) + vec2(0.5);
	vec2 invTexSize = 1.0 / texSize;
	return blTexture(texCoord * invTexSize, texSize);
}

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
        vec2 texSize = textureSize(defaultTexture, 0);
		vec4 sampledColor = nnTexture(inBlock.uv * inSize / texSize, texSize);
		//if ( inBlock.uv.x >= 0.0 && inBlock.uv.x <= 1.0 && inBlock.uv.y >= 0.0 && inBlock.uv.y <= 1.0)
			outColor = sampledColor;

	} 

	else
	{
		outColor = texture(defaultTexture, inBlock.uv);
	}	
}