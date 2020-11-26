#version 440

in defaultBlock
{
	vec4 position;
    vec4 normal;
	vec2 uv;
	
} inBlock;

out float outColor;

layout(binding = 0) uniform sampler2D diffuse;
void main()
{
		//clamp the alpha down hard. if alpha is less than 0.1, clamp it to 0
	vec4 col =  texture(diffuse, inBlock.uv);

	if(col.a < 0.5)
	{
		discard;
	}

	else
	{
		col.a = 1;
	}

	outColor = 1;
}