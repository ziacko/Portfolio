#version 420

layout(triangles, fractional_even_spacing, cw) in;

in tessBlock
{
	vec4 		position;
	vec4 		normal;
	vec2		uv;
} inBlock[];

out defaultBlock
{
    vec4 		position;
	vec4 		normal;
	vec2		uv;
} outBlock;

layout(std140, binding = 1) uniform displacementSettings
{
    float outTessLevel;
    float inTessLevel;
	float offsetStrength;
};

layout(binding = 1) uniform sampler2D displacementMap;

void main()
{
    vec4 p0 = gl_in[ 0 ].gl_Position;
	vec4 p1 = gl_in[ 1 ].gl_Position;
	vec4 p2 = gl_in[ 2 ].gl_Position;
	vec3 p = gl_TessCoord.xyz;

    gl_Position = p0 * p.x + p1 * p.y + p2 * p.z;

	vec4 position = (inBlock[0].position * p.x) + (inBlock[1].position * p.y) + (inBlock[2].position * p.z);
	vec4 normal = (inBlock[0].normal * p.x) + (inBlock[1].normal * p.y) + (inBlock[2].normal * p.z);
	vec2 uv = (inBlock[0].uv * p.x) + (inBlock[1].uv * p.y) + (inBlock[2].uv * p.z);

	vec4 displacement = texture2D(displacementMap, uv);
	float tessStrength = mix(1, 10, displacement.r);
	float test = smoothstep(1, 10, tessStrength); //redundant

	//if(displacement.r > 0)
	{
		gl_Position.y += normal.y * (displacement.r * offsetStrength);
	}

    outBlock.position = gl_Position;
    outBlock.normal = normal;
    outBlock.uv = uv;
}