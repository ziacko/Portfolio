#version 420

layout(triangles, fractional_even_spacing, cw) in;

in tessBlock
{
	vec4 		worldPosition;
	vec4 		screenPosition;
	vec4 		normal;
	vec2		uv;
} inBlock[];

out defaultBlock
{
    vec4 		worldPosition;
	vec4 		screenPosition;
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
	//gl_InvocationID is which vertex i am?
	/*vec2 tc1 = mix(inBlock[0].uv, inBlock[1].uv, gl_TessCoord.x);
	vec2 tc2 = mix(inBlock[2].uv, inBlock[3].uv, gl_TessCoord.x);
	vec2 tc = mix(tc2, tc1, gl_TessCoord.y);

	vec4 p1 = mix(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x);
	vec4 p2 = mix(gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);
	vec4 p = mix(p2, p1, gl_TessCoord.y);

	p.y += texture(displacementMap, outBlock[gl_InvocationID].uv).r;
*/
    vec4 p0 = gl_in[ 0 ].gl_Position;
	vec4 p1 = gl_in[ 1 ].gl_Position;
	vec4 p2 = gl_in[ 2 ].gl_Position;
	vec3 p = gl_TessCoord.xyz;

    gl_Position = p0 * p.x + p1 * p.y + p2 * p.z;

    vec4 worldPos = (inBlock[0].worldPosition * p.x) + (inBlock[1].worldPosition * p.y) + (inBlock[2].worldPosition * p.z);
	vec4 position = (inBlock[0].screenPosition * p.x) + (inBlock[1].screenPosition * p.y) + (inBlock[2].screenPosition * p.z);
	vec4 normal = (inBlock[0].normal * p.x) + (inBlock[1].normal * p.y) + (inBlock[2].normal * p.z);
	vec2 uv = (inBlock[0].uv * p.x) + (inBlock[1].uv * p.y) + (inBlock[2].uv * p.z);

	vec4 displacement = texture2D(displacementMap, uv);
	float tessStrength = mix(1, 10, displacement.r);
	float test = smoothstep(1, 10, tessStrength); //redundant

	//if(displacement.r > 0)
	{
		gl_Position.y += normal.y * (displacement.r * offsetStrength);
		worldPos.y += normal.y * (displacement.r * offsetStrength);
	}

	outBlock.worldPosition = worldPos;
    outBlock.screenPosition = gl_Position;
    outBlock.normal = normal;
    outBlock.uv = uv;
}