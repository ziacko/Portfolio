#pragma once

struct wavesCascade_t
{
	unsigned int size;
	unsigned int initialSpectrumShader;
	unsigned int timeDependentSpectrumShader;
	unsigned int texturesMergerShader;
	FFTransform_t fft;
	texture gaussianNoise;
	//ComputeBuffer paramsBuffer; SSBO for compute shaders?
	unsigned int initialSpectrum;
	unsigned int precomputedData;

	unsigned int buffer;
	unsigned int DxDz;
	unsigned int DyDxz;
	unsigned int DyxDyz;
	unsigned int DxxDzz;

	unsigned int displacement;
	unsigned int derivatives;
	unsigned int turbulence;

	float lambda;

	wavesCascade_t(unsigned int size, 
		unsigned int initSpectrumProgram,
		unsigned int timeDepSpectrumProgram,
		unsigned int texMergerProgram,
		)
	{

	}



};
