#version 450

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec4 biTangent;
layout (location = 4) in vec2 uv;

out defaultBlock
{
	vec4 		position;
	vec2		uv;
} outBlock;

out lightBlock
{
	vec4 normal;
	vec4 position;
	vec4 view;

} outLight;

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

layout(std140, binding = 2) uniform lightSettings
{
	vec4		color;
	vec4		direction;
	vec4		position;
	float		intensity;
	float		angle;
	float		range;

	uint		currentType;
} defaultLight;

void main()
{
	outBlock.position = projection * view * translation * position;
	outBlock.uv = uv;

	vec4 lightPosition = translation * position;
	outLight.normal = translation * normalize(position);
	outLight.position = defaultLight.position - lightPosition;
	//rotate light position by direction?
	outLight.view = defaultLight.direction;

	
	gl_Position = outBlock.position;
}
