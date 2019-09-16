#version 420

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

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

layout(std140, binding = 4) 
uniform freiChenSettings
{
	float		filterLevel;
};

uniform sampler2D defaultTexture;

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
		vec2 texel = vec2(1.0 / resolution.x, 1.0 / resolution.y);
		//if I were to put all these into imGUI it would be about 81 (9x9) uniforms!
		mat3 G[9] = 
		{
			mat3( 0.3535533845424652, 0, -0.3535533845424652, 0.5, 0, -0.5, 0.3535533845424652, 0, -0.3535533845424652 ),
			mat3( 0.3535533845424652, 0.5, 0.3535533845424652, 0, 0, 0, -0.3535533845424652, -0.5, -0.3535533845424652 ),
			mat3( 0, 0.3535533845424652, -0.5, -0.3535533845424652, 0, 0.3535533845424652, 0.5, -0.3535533845424652, 0 ),
			mat3( 0.5, -0.3535533845424652, 0, -0.3535533845424652, 0, 0.3535533845424652, 0, 0.3535533845424652, -0.5 ),
			mat3( 0, -0.5, 0, 0.5, 0, 0.5, 0, -0.5, 0 ),
			mat3( -0.5, 0, 0.5, 0, 0, 0, 0.5, 0, -0.5 ),
			mat3( 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.6666666865348816, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204 ),
			mat3( -0.3333333432674408, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, 0.6666666865348816, 0.1666666716337204, -0.3333333432674408, 0.1666666716337204, -0.3333333432674408 ),
			mat3( 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408, 0.3333333432674408 ),
		};

		mat3 I;
		float cnv[9];
		vec3 tex;

		for(float i = 0.0; i < 3.0; i++)
		{
			for(float j = 0.0; j < 3.0; j++)
			{
				tex = texture2D(defaultTexture, inBlock.uv + texel * vec2(i - 1.0, j - 1.0)).rgb;
				I[int(i)][int(j)] = length(tex);
			}
		}

		for(int i = 0; i < 9; i++)
		{
			float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]) * filterLevel;
			cnv[i] = dp3 * dp3;
		}

		float M = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]);
		float S = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + (cnv[8] + M);

		outColor = vec4(vec3(sqrt(M/S)), 1);
	}

	else
	{
		outColor = texture2D(defaultTexture, inBlock.uv);
	}
}