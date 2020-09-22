#pragma once
class Compressor : public  SoLoud::Filter
{
public:
	virtual SoLoud::FilterInstance* CreateInstance();
};

class CompressorInstance : public SoLoud::FilterInstance
{
public:

	virtual SoLoud::result initParams(int aNumParams);

	virtual void updateParams(SoLoud::time aTime);

	virtual void filter(float* aBuffer, unsigned int aSamples, unsigned int aBufferSize, unsigned int aChannels, float aSamplerate, SoLoud::time aTime);

	virtual void filterChannel(float* aBuffer, unsigned int aSamples, float aSamplerate, SoLoud::time aTime, unsigned int aChannel, unsigned int aChannels);

	virtual float getFilterParameter(unsigned int aAttributeId);

	virtual void setFilterParameter(unsigned int aAttributeId, float aValue);

	virtual void fadeFilterParameter(unsigned int aAttributeId, float aTo, SoLoud::time aTime, SoLoud::time aStartTime);

	virtual void oscillateFilterParameter(unsigned int aAttributeId, float aFrom, float aTo, SoLoud::time aTime, SoLoud::time aStartTime);

	float RootMeanSquare(float* block, int num);

	void SetThresh(float thresh);

	void SetRatio(float ratio);



private:

	int sampleRate;
	int blockSize;

	float coefficient;
	float sample;
	float thresh;
	float ratio;
	float attack;
	float release;
	int attackFlag;
	float gainChange;
	float gainTarget;
	int fadeSamples;
};
