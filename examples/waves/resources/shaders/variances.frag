#version 420

uniform float numSlopeVariance;
uniform int fftSize;
uniform float slopeVarianceDelta;
uniform float c;
uniform vec4 gridSizes;

uniform sampler2D spectrum12Sampler;
uniform sampler2D spectrum34Sampler;

in vec2 uv;

layout(location = 0) out vec2 outColor;

const float pi = 3.14159265f;

vec2 GetSlopeVariances(vec2 k, float a, float b, float c, vec2 spectrumSample)
{
	float w = 1.f - exp(a * k.x * k.x + b * k.x * k.y + c * k.y * k.y);
	vec2 kw = k * w;
	return kw * kw * dot(spectrumSample, spectrumSample) * 2.f;
}

void main()
{
	const float scale = 10.f;

	float a = floor(uv.x * numSlopeVariance);
	float b = floor(uv.y * numSlopeVariance);
	float A = pow(a / (numSlopeVariance - 1.f), 4.f) * scale;
	float C = pow(c / (numSlopeVariance - 1.f), 4.f) * scale;
	float B = (2.f * b / (numSlopeVariance - 1.f) - 1.f) * sqrt(A * C);
	A = -0.5f * A;
	B = -B;
	C = -0.5f * C;

	vec2 slopeVariances = vec2(slopeVarianceDelta);
	for(int y = 0; y < fftSize; ++y)
	{
		for(int x = 0; x < fftSize; x++)
		{
			int i = x >= fftSize / 2 ? x - fftSize : x;
			int j = y >= fftSize / 2 ? y - fftSize : y;
			vec2 k = 2.f * pi * vec2(i, j);

			vec4 spectrum12 = texture2D(spectrum12Sampler, vec2(float(x) + 0.5f, float(y) + 0.5f) / float(fftSize));
			vec4 spectrum34 = texture2D(spectrum34Sampler, vec2(float(x) + 0.5f, float(y) + 0.5f) / float(fftSize));

			slopeVariances += GetSlopeVariances(k / gridSizes.x, A, B, C, spectrum12.xy);
			slopeVariances += GetSlopeVariances(k / gridSizes.y, A, B, C, spectrum12.zw);
			slopeVariances += GetSlopeVariances(k / gridSizes.z, A, B, C, spectrum34.xy);
			slopeVariances += GetSlopeVariances(k / gridSizes.w, A, B, C, spectrum34.zw);
		}
	}
	outColor = slopeVariances.xy;
}