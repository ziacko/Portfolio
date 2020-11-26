#version 450

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

layout(binding = 1) uniform perlin3DSettings
{
    vec3  uvwScale;
    int   layer;
};

layout(binding = 0) uniform sampler3D defaultTexture;

void main()
{
    vec3 texSize = textureSize(defaultTexture, 0);
    float wLayer = (layer / texSize.z) * 0.5 + 0.5;
	outColor = texture(defaultTexture, vec3(inBlock.uv, wLayer));
}