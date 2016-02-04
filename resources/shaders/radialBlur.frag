#version 440

in defaultBlock
{
	vec4		position;
	vec2		UV;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};

layout(std140, binding = 1) uniform radialSettings
{
	float		exposure;
	float		decay;
	float		density;
	float		weight;
	uint		samples;
};

uniform sampler2D defaultTexture;

vec4 radialBlur()
{
	vec4 color = texture2D(defaultTexture, inBlock.UV);
	vec2 deltaCoord = vec2(gl_FragCoord.x - mousePosition.x, mousePosition.y - gl_FragCoord.y);
	deltaCoord *= 1.0 /  float(samples) * density;
	float startDecay = 1.0;
	
	vec2 uv = inBlock.UV;

	for(int i = 0; i < samples ; i++)
	{
		uv -= deltaCoord;

		vec4 radialTex = texture2D(defaultTexture, uv );
		radialTex *= startDecay * weight;
		color += radialTex;
		startDecay *= decay;
	}

	color *= exposure;
	return color;
}

void main()
{
	outColor = texture2D(defaultTexture, inBlock.UV) + radialBlur();
}
