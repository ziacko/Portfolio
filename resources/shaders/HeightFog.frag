#version 420

in defaultBlock
{
	vec4 		worldPosition;
	vec4 		screenPosition;
    vec4        normal;
	vec2		uv;
} inBlock;

out vec4 outColor;

layout(binding = 0) uniform defaultSettings
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

layout(std140, binding = 2) uniform fogSettings
{
	vec4 fogColor;
	vec4 des;
	vec4 dis;	
};

uniform sampler2D diffuse;

vec4 fog(vec4 color)
{
	float z = 1 - inBlock.screenPosition.z / inBlock.screenPosition.w;
	
	float de = des.x * smoothstep(0.0, des.y, des.z - inBlock.worldPosition.y);
	float di = dis.x * smoothstep(0.0, dis.y, dis.z - inBlock.worldPosition.y);

	float extinction = exp(-z * de);
	float inscattering = exp(-z * di);

	return color * extinction + fogColor * (1.0 - inscattering);
}

//basic distance fog
vec4 fog2(vec4 color)
{

	//or (view * inBlock.WorldPosition).z
	//float dist = -(view * inBlock.worldPosition).z; //need camera to point distance
	float dist =  1 - inBlock.screenPosition.z / inBlock.screenPosition.w;
	//float dist = -viewSpacePosition.z;


	float amount = 1.0 - exp(-dist * des.x); //wtf is b? b is extinction?
	//return mix(color, fogColor, amount);
	vec4 final = color * amount + fogColor * exp(-dist * des.x);
	return final;
}

vec4 fog3(in vec4 color)
{	
	float dist = 1 - inBlock.screenPosition.z / inBlock.screenPosition.w;
	//float dist = (view * inBlock.worldPosition).z;
	vec3 extinction = vec3(exp(-dist * des.x), exp(-dist * des.y), exp(-dist* des.z));
	vec3 inscattering = vec3(exp(-dist * dis.x), exp(-dist * dis.y), exp(-dist* dis.z));
	vec3 output = color.rgb * (1.0 - extinction) + fogColor.rgb * inscattering;

	return vec4(output, 1);
}

vec4 fog4(in vec4 color)
{
	vec4 csPos = view * inBlock.worldPosition;

	float dist = inBlock.screenPosition.z / inBlock.screenPosition.w;
	vec4 rayStart = view[3];
	vec4 rayDir = inBlock.worldPosition - inverse(view)[2];

	//wtf is c, b?
	float fogAmount = des.x * exp(-rayStart.y * des.y) * (1.0 - exp(-dist * rayDir.y * des.y)); //rayDir.y 
	fogAmount = clamp(fogAmount, 0, 1);
	return mix(color, fogColor, 1 - fogAmount);
	//return inverse(view)[2];
}

void main()
{
	outColor = fog4(texture2D(diffuse, inBlock.uv));
}