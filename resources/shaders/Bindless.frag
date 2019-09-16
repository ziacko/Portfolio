#version 440
#extension GL_ARB_bindless_texture : require

in defaultBlock
{
	vec4 position;
	vec2 uv;
} inBlock;

out vec4 outColor;

layout(binding = 1) uniform textures
{
    sampler2D defaultTexture;
};

void main()
{
	outColor = texture2D(defaultTexture, inBlock.uv);
    //outColor = vec4(1, 0, 0, 1);
}