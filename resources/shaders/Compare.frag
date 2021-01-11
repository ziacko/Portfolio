#version 440

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

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

out vec4 outColor;

layout(binding = 0) uniform sampler2D defaultTexture;
layout(binding = 1) uniform sampler2D compareTexture;

// (xchen) linear to gamma sRGB transformation
const float SRGB_GAMMA = 1.0 / 2.2;
const float SRGB_INVERSE_GAMMA = 2.2;
const float SRGB_ALPHA = 0.055;

// (xchen) linear to gamma sRGB transformation
// Converts a single linear channel to srgb
float linear_to_srgb(float channel) {
    if(channel <= 0.0031308)
        return 12.92 * channel;
    else
        return (1.0 + SRGB_ALPHA) * pow(channel, 1.0/2.4) - SRGB_ALPHA;
}

// (xchen) linear to gamma sRGB transformation
// Converts a linear rgb color to a srgb color (exact, not approximated)
vec3 rgb_to_srgb(vec3 rgb) {
    return vec3(
        linear_to_srgb(rgb.r),
        linear_to_srgb(rgb.g),
        linear_to_srgb(rgb.b)
    );
}

void main()
{
	vec4 defaultColor = texture2D(defaultTexture, inBlock.uv);
	vec4 compareColor = texture2D(compareTexture, inBlock.uv);

	if(gl_FragCoord.x < mousePosition.x)
	{
		// (xchen) linear to gamma sRGB transformation
		outColor.xyz = rgb_to_srgb( defaultColor.xyz );
	}
	else
	{
		// (xchen) linear to gamma sRGB transformation
		outColor.xyz = rgb_to_srgb( compareColor.xyz );
	}
	outColor.w = 1.0f;
}