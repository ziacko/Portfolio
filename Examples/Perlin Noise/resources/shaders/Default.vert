#version 440

out vec4 vPosition;
out vec2 vUV;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	ivec2		resolution;
};

void main()
{
	vec4 quad[4] = vec4[4](
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(resolution.x, 0.0f, 1.0f, 1.0f),
		vec4(resolution.x, resolution.y, 1.0f, 1.0f),
		vec4(0.0f, resolution.y, 1.0f, 1.0f));

	//generate the render quad from here, don't event bother with a vertex buffer
	for(int iter = 0; iter < 4; iter++)
	{
		quad[iter] = projection * view * translation * quad[iter];
	}

	vUV = quad[gl_VertexID].xy;
	gl_Position = quad[gl_VertexID];
}