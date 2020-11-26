#version 420

in defaultBlock
{
	vec4 		position;
	vec2		uv;
} inBlock;

in lightBlock
{
	vec4 normal;
	vec4 position;
	vec4 view;

} inLight;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4 		view;
	mat4 		translation;
	vec2		resolution;
	vec2		mousePosition;
	float		deltaTime;
	float		totalTime;
	float 		framesPerSecond;
	uint 		totalFrames;
};

layout(std140, binding = 1) uniform materialSettings
{
	vec4 		diffuseMat;
	vec4		specularMat;
	vec4		ambientMat;
	vec4		emissiveMat;
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

uniform sampler2D diffuse;

void main()
{
	//vec4 norm = normalize(inBlock.position);
	//vec4 diff = texture2D(diffuse, inBlock.uv);

	//float dotprod = max(0.0, dot(norm, defaultLight.direction));

	vec4 N = normalize(inLight.normal);
	vec4 L = normalize(inLight.position);
	vec4 V = normalize(inLight.view);

	vec4 R = reflect(-L, N);


	float dotprod = max(dot(N, L), 0.0);
	
	outColor = texture(diffuse, inBlock.uv) + ((defaultLight.color * defaultLight.intensity) * dotprod);
}