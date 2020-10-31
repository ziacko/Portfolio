#version 420

layout (vertices = 3) out;

in defaultBlock
{
	vec4 		position;
	vec4 		normal;
	vec2		uv;
} inBlock[];

out tessBlock
{
    vec4 		position;
	vec4 		normal;
	vec2		uv;
} outBlock[];

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
	uint 		totalFrames;
};

layout(std140, binding = 1) uniform displacementSettings
{
    float outTessLevel;
    float inTessLevel;
	float offsetStrength;
};

layout(binding = 1) uniform sampler2D displacementMap;

void main()
{
    //gl_InvocationID is which vertex i am?

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;

    outBlock[gl_InvocationID].position = inBlock[gl_InvocationID].position;
    outBlock[gl_InvocationID].normal = inBlock[gl_InvocationID].normal;
    outBlock[gl_InvocationID].uv = inBlock[gl_InvocationID].uv;

	//since this is a 10*10 grid, using Uvs to map where to draw from would lead to very random texels

	//ok sample the displacement map, tesselate based on strength of displacement?

	//or sample the surrounding texels, if the difference is high enough, boost tesselation
	//that way we don't over-tesselate areas

	vec4 displacement = texture2D(displacementMap, outBlock[gl_InvocationID].uv);
	float tessStrength = 1;
	if(displacement.r < 1 && displacement.r > 0)
	{
		tessStrength = mix(1, 10, displacement.r);
	}	

    gl_TessLevelOuter[0] = outTessLevel;
	gl_TessLevelOuter[1] = outTessLevel;
	gl_TessLevelOuter[2] = outTessLevel;
	gl_TessLevelOuter[3] = outTessLevel;

	gl_TessLevelInner[0] = tessStrength;
    gl_TessLevelInner[1] = tessStrength;
}