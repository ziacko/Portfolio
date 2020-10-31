#version 420
#define SMAA_DECODE_VELOCITY(sample) sample.rg
#define SMAA_REPROJECTION_WEIGHT_SCALE 80.0

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

layout(std140, binding = 1)
uniform sobelSettings
{
	float		redModifier;
	float		greenModifier;
	float		blueModifier;
	float		cellDistance;
};

layout(binding = 0) uniform sampler2D currentColorTex;
layout(binding = 1) uniform sampler2D currentDepthTex;

layout(binding = 2) uniform sampler2D previousColorTex;
layout(binding = 3) uniform sampler2D previousDepthTex;

layout(binding = 4) uniform sampler2D velocityTex;

//-----------------------------------------------------------------------------
// Temporal Resolve Pixel Shader (Optional Pass)

vec4 SMAAResolvePS(vec2 texcoord)
{
    // Velocity is assumed to be calculated for motion blur, so we need to
    // inverse it for reprojection:
    vec2 velocity = -texture(velocityTex, texcoord).rg;

    // Fetch current pixel:
    vec4 current = texture(currentColorTex, texcoord);

    // Reproject current coordinates and fetch previous pixel:
    vec4 previous = texture(previousColorTex, texcoord + velocity);

    // Attenuate the previous pixel if the velocity is different:
    float delta = abs(current.a * current.a - previous.a * previous.a) / 5.0;
    float weight = 0.5 * clamp(1.0 - sqrt(delta) * SMAA_REPROJECTION_WEIGHT_SCALE, 0.0, 1.0);

    // Blend the pixels according to the calculated weight:
    return mix(current, previous, weight);
	//return mix(current, previous, 0.5);
}

void main()
{
    outColor = SMAAResolvePS(inBlock.uv);
}