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

layout(binding = 0) uniform sampler2DMS MSAATex;

void main()
{
    vec4 result = vec4(0.0);
    ivec2 coord = ivec2(gl_FragCoord.xy);
    for(int i = 0; i < 8; i++)
    {
        result += texelFetch(MSAATex, coord, i);
    }

	outColor = result / 8;
}