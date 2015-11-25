#version 440

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 UV;

out vec4 vPosition;
out vec2 vUV;
out vec4 vColor;

#define MAX_BUFFER 1000

layout(binding = 0) uniform defaultSettings {
	mat4 Projection;
	mat4 View;
	mat4 Model;
	float Width;
	float Height;
};

layout(packed, binding = 1) uniform GOLsettings {
	vec4 AliveColor;
	vec4 DeadColor;
	vec4 EmptyColor;	
	int Dimensions;
};

layout(std430, binding = 0) buffer GOLstatus {
	int Status[];
};

void main()
{
	vPosition = Projection * View * Model * Position;
	vUV = UV;

	//float XResult = 0;
	//float YResult = modf(gl_InstanceID / Dimensions, XResult); 
	//vPosition.x = vPosition.x + (XResult * ((Width / Dimensions.x) / Width)) * 2;
	//vPosition.y = vPosition.y + YResult * 2;

	if(Status[gl_InstanceID] == 0)
	{
		vColor = EmptyColor;
	}

	if(Status[gl_InstanceID] == 1)
	{
		vColor = AliveColor;
	}

	if(Status[gl_InstanceID] == 2)
	{
		vColor = DeadColor;
	}

	gl_Position = vPosition;
}