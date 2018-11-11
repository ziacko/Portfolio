#version 420

in defaultBlock
{
	vec4 position;
	vec2 UV;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0)
uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};

layout(std140, binding = 3) 
uniform prewittSettings
{
	float		filterLevel;
};

uniform sampler2D defaultTexture;

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
		float widthStep = 1.0 / resolution.x;
		float heightStep = 1.0 / resolution.y;
		vec2 offsets[9];
		vec4 samples[9];

		offsets[0] = vec2(-widthStep, -heightStep);
		offsets[1] = vec2(0.0, -heightStep);
		offsets[2] = vec2(widthStep, -heightStep);	   
		offsets[3] = vec2(-widthStep, 0.0);
		offsets[4] = vec2(0.0, 0.0);
		offsets[5] = vec2(widthStep, 0.0);	   
		offsets[6] = vec2(-widthStep, heightStep);
		offsets[7] = vec2(0.0, heightStep);
		offsets[8] = vec2(widthStep, heightStep);

		for(int sampleIndex = 0; sampleIndex < 9; sampleIndex++)
		{
			samples[sampleIndex] = texture2D(defaultTexture, inBlock.UV + offsets[sampleIndex]);
		}

		vec4 horzontalEdge = samples[2] + samples[5] + samples[8] - (samples[0] + samples[3] + samples[6]);
		vec4 verticalEdge = samples[0] + samples[1] + samples[2] - (samples[6] + samples[7] + samples[8]);
		vec4 color = sqrt((horzontalEdge * horzontalEdge) + (verticalEdge * verticalEdge));

		if(length(color) > filterLevel)
		{
			outColor = vec4(1);
		}

		else
		{
			outColor = vec4(0);
		}
	}

	else
	{
		outColor = texture2D(defaultTexture, inBlock.UV);
	}	
}