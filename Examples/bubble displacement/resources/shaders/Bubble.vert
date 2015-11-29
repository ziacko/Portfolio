#version 440

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uV;

out vec4 vPosition;
out vec2 vUV;

layout(binding = 0) uniform defaultSettings
{
	mat4 projection;
	mat4 view;
	mat4 translation;
	float width;
	float height;
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
	vPosition = projection * view * translation * position;
	vec4 mouse = projection * view * translation * vec4(1, 1, 1, 1);
	//vec4 Res = projection * view * translation * vec4(width, height, 0, 1);
	float Distance = length(vec2(1, 1) - vPosition.xy);

	//if the distance between the vert and mouse position is less than attenuation then push said vert away from the mouse Position
	if( Distance < attenuation)
	{
		vPosition.xy += normalize(vec2(1, 1) - vPosition.xy) * Distance;
	}
	vUV = position.xy / vec2(width, height);
	gl_Position = vPosition;
}