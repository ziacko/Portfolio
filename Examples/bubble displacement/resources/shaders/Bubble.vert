#version 440

out vec4 vPosition;
out vec2 vUV;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4 projection;
	mat4 view;
	mat4 translation;
	vec2 resolution;
};

layout (std140, binding = 1) uniform bubbleSettings
{
	vec2 mousePosition;
	float attenuation;
	float offset;
	float dimensions;
};

void main()
{
	float cellWidth = resolution.x / dimensions;
	float cellHeight = resolution.y / dimensions;

	float x = 0;
	float y = modf(gl_InstanceID / dimensions, x);

	vec4 quad[6] = vec4[6](
		vec4(0.0f, 0.0f, 1.0f, 1.0f),
		vec4(cellWidth, 0.0f, 1.0f, 1.0f),
		vec4(0.0f, cellHeight, 1.0f, 1.0f),

		vec4(0.0f, cellHeight, 1.0f, 1.0f),
		vec4(cellWidth, 0.0f, 1.0f, 1.0f),
		vec4(cellWidth, cellHeight, 1.0f, 1.0f));

	vec4 mouseWindowPosition = projection * view * translation * vec4(mousePosition.x, mousePosition.y, 1, 1);

	//generate the render quad from here, don't event bother with a vertex buffer
	for(int iter = 0; iter < 6; iter++)
	{
		quad[iter] = projection * view * translation * quad[iter];
		float x = 0;
		float y = modf(gl_InstanceID / dimensions, x);
		quad[iter].x = quad[iter].x + (x * (cellWidth) / resolution.x) * 2;
		quad[iter].y = quad[iter].y - (y * (cellHeight) / resolution.y) * (dimensions * 2);
	}

	float distancefromMouse = length(mouseWindowPosition.xy - quad[gl_VertexID].xy);

	if(distancefromMouse < attenuation)
	{
		quad[gl_VertexID].xy += normalize(mouseWindowPosition.xy - quad[gl_VertexID].xy) * distancefromMouse * offset;
	}

	vPosition = quad[gl_VertexID];
	vUV = quad[gl_VertexID].xy / resolution * 500.0f + 0.6f;
	gl_Position = vPosition;
}