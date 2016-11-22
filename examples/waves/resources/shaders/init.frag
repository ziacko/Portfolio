#version 420
uniform sampler2D spectrum12Sampler;
uniform sampler2D spectrum34Sampler;

uniform float fftSize;
uniform vec4 inverseGridSizes;
uniform float t;

in vec2 uv;

layout(location = 0) out vec4 layer0;
layout(location = 1) out vec4 layer1;
layout(location = 2) out vec4 layer2;
layout(location = 3) out vec4 layer3;
layout(location = 4) out vec4 layer4;

vec2 GetSpectrum(float k, vec2 s0, vec2 s0c)
{
	float w = sqrt(9.81f * k * (1.0f + k * k / (370.f, 370.f)));
	float c = cos(w * t);
	float s = sin(w * t);
	return vec2((s0.x + s0c.x) * c - (s0.y + s0c.y) * s, (s0.x - s0c.x) * s + (s0.y - s0c.y) * c);
}

vec2 i(vec2 z)
{
	return vec2(-z.y, z.x);
}

void main()
{
	vec2 st = floor(uv * fftSize) / fftSize;
	float x = uv.x > 0.5f ? st.x - 1.f : st.x;
	float y = uv.y > 0.5f ? st.y - 1.f : st.y;

	vec4 s12 = texture2DLod(spectrum12Sampler, uv, 0.f);
	vec4 s34 = texture2DLod(spectrum34Sampler, uv, 0.f);
	vec4 s12c = texture2DLod(spectrum12Sampler, vec2(1.f + 0.5f / fftSize) - st, 0.f);
	vec4 s34c = texture2DLod(spectrum34Sampler, vec2(1.f + 0.5f / fftSize) - st, 0.f);

	vec2 k1 = vec2(x, y) * inverseGridSizes.x;
	vec2 k2 = vec2(x, y) * inverseGridSizes.y;
	vec2 k3 = vec2(x, y) * inverseGridSizes.z;
	vec2 k4 = vec2(x, y) * inverseGridSizes.w;

	float K1 = length(k1);
	float K2 = length(k2);
	float K3 = length(k3);
	float K4 = length(k4);

	float IK1 = K1 == 0.f ? 0.f : 1.0f / K1;
	float IK2 = K2 == 0.f ? 0.f : 1.0f / K2;
	float IK3 = K3 == 0.f ? 0.f : 1.0f / K3;
	float IK4 = K4 == 0.f ? 0.f : 1.0f / K4;

	vec2 h1 = GetSpectrum(K1, s12.xy, s12c.xy);
	vec2 h2 = GetSpectrum(K2, s12.zw, s12c.zw);
	vec2 h3 = GetSpectrum(K3, s34.xy, s34c.xy);
	vec2 h4 = GetSpectrum(K4, s34.zw, s34c.zw);

	layer0 = vec4(h1 + i(h2), h3 + i(h4));
	layer1 = vec4(i(k1.x * h1) - k1.y * h1, i(k2.x * h2) - k2.y * h2); 
	layer2 = vec4(i(k3.x * h3) - k3.y * h3, i(k4.x * h4) - k4.y * h4);
	layer3 = layer1 * vec4(IK1, IK1, IK2, IK2);
	layer4 = layer2 * vec4(IK3, IK3, IK4, IK4);
}