#version 420

in defaultBlock
{
	vec4		position;
	vec2		UV;
} inBlock;

out vec4 outColor;

layout(std140, binding = 0) uniform defaultSettings
{
	mat4		projection;
	mat4		view;
	mat4		translation;
	vec2		resolution;
	vec2		mousePosition;
	double		deltaTime;
	double		totalTime;
};

layout(std140, binding = 1) uniform parallaxSettings
{
	float		redModifier;
	float		greenModifier;
	float		blueModifier;
	float		cellDistance;
};

uniform sampler2D defaultTexture;

float luma( vec4 color, float RedMod, float GreenMod, float BlueMod)
{
	//you can opt to use these Mod value to tweak the shader to your liking.
	return RedMod * color.r + GreenMod * color.g + BlueMod * color.b;
}

void main()
{
	if(gl_FragCoord.x < mousePosition.x)
	{
		float deltaY = 1.0f / resolution.y;
		float deltaX = 1.0f / resolution.x;

		float s00 = luma(texture(defaultTexture, inBlock.UV + vec2(-deltaX, deltaY)), redModifier, greenModifier, blueModifier);
		float s10 = luma(texture(defaultTexture, inBlock.UV + vec2(-deltaX, 0.0)), redModifier, greenModifier, blueModifier);
		float s20 = luma(texture(defaultTexture, inBlock.UV + vec2(-deltaX, 0.0)), redModifier, greenModifier, blueModifier);
		float s01 = luma(texture(defaultTexture, inBlock.UV + vec2(0.0, deltaY)), redModifier, greenModifier, blueModifier);
		float s21 = luma(texture(defaultTexture, inBlock.UV + vec2(0.0, -deltaY)), redModifier, greenModifier, blueModifier);
		float s02 = luma(texture(defaultTexture, inBlock.UV + vec2(deltaX, deltaY)), redModifier, greenModifier, blueModifier);
		float s12 = luma(texture(defaultTexture, inBlock.UV + vec2(deltaX, 0.0)), redModifier, greenModifier, blueModifier);
		float s22 = luma(texture(defaultTexture, inBlock.UV + vec2(deltaX, -deltaY)), redModifier, greenModifier, blueModifier);

		float Kernel1 = s00 + 2 * s10 + s20 - (s02 + 2 * s12 + s22);
		float Kernel2 = s00 + 2 * s01 + s02 - (s20 + 2 * s21 + s22);
		//get the square root of Kernel1 squared and Kernel2 squared
		float dist = sqrt((Kernel1 * Kernel1) + (Kernel2 * Kernel2));
		if (dist > cellDistance )
		{
			outColor = vec4(0.0);
		}

		else
		{
			outColor = texture2D(defaultTexture, inBlock.UV);
		}
	}

	else
	{
		outColor = texture2D(defaultTexture, inBlock.UV);
	}
}