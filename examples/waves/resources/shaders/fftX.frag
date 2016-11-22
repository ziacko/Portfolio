#version 420

uniform sampler2D butterflySampler;
uniform sampler2DArray imageSampler;

uniform float pass;
uniform int layer;

in vec2 uvGeom;

layout(location = 0) out vec4 outColor;

vec4 fftX(int layer, vec2 i, vec2 w)
{
	vec4 input1 = texture2DArrayLod(imageSampler, vec3(i.x, uvGeom.y, layer), 0.0f);
	vec4 input2 = texture2DArrayLod(imageSampler, vec3(i.y, uvGeom.y, layer), 0.0f);
	float res1x = w.x * input2.x - w.y * input2.y;
	float res1y = w.y * input2.x + w.x * input2.y;
	float res2x = w.x * input2.z - w.y * input2.w;
	float res2y = w.y * input2.z + w.x * input2.w;
	return input1 + vec4(res1x, res1y, res2x, res2y);
}

void main()
{
	vec4 data = texture2DLod(butterflySampler, vec2(uvGeom.x, pass), 0.0f);
	vec2 i = data.xy;
	vec2 w = data.zw;
	outColor = fftX(gl_PrimitiveID, i, w);
}