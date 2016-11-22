#version 420
layout (location = 0) in vec4 position;

out vec2 uv;

void main()
{
	uv = position.xy * 0.5f + 0.5f;
	gl_Position = position;
}