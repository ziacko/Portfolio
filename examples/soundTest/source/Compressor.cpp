#define GLM_ENABLE_EXPERIMENTAL

#include "Compressor.h"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <vector>
#include <cmath>
#include <chrono>

/// <summary>
/// Compressor Instance functions
/// </summary>
/// <param name="parent"></param>
/// <returns></returns>

using namespace std::chrono;
namespace SoLoud
{
	//TODO:
	//set filter parameter function
	//fade and oscillate function

	CompressorInstance::CompressorInstance(CompressionFilter* parent) : FilterInstance()
	{
		//probably not necessary at all
		this->parent = parent;
		initParams(31);
		mParam[WET] = parent->wet;
		mParam[KNEE] = parent->knee;
		mParam[THRESHOLD] = parent->threshold;
		mParam[PREGAIN] = parent->pregain;
		mParam[RATIO] = parent->ratio;
		mParam[ATTACK] = parent->attack;
		mParam[RELEASE] = parent->release;
		mParam[RELEASEZONE1] = parent->releaseZone1;
		mParam[RELEASEZONE2] = parent->releaseZone2;
		mParam[RELEASEZONE3] = parent->releaseZone3;
		mParam[RELEASEZONE4] = parent->releaseZone4;
		mParam[POSTGAIN] = parent->postGain;

		//should be read only
		mParam[METER_GAIN] = parent->meterGain;
		mParam[METER_RELEASE] = parent->meterRelease;
		mParam[LINEAR_PREGAIN] = parent->linearPregain;
		mParam[LINEAR_THRESHOLD] = parent->linearThreshold;
		mParam[SLOPE] = parent->slope;
		mParam[ATTACK_SAMPLES_INV] = parent->attackSamplesInv;
		mParam[SAT_RELEASE_SAMPLES_INV] = parent->satReleaseSamplesInv;
		mParam[DRY] = parent->dry;
		mParam[KNEE_DB_OFFSET] = parent->kneeDBOffset;
		mParam[LINEAR_THRESHOLD_KNEE] = parent->linearThresholdKnee;
		mParam[MASTER_GAIN] = parent->masterGain;
		mParam[A] = parent->a;
		mParam[B] = parent->b;
		mParam[C] = parent->c;
		mParam[D] = parent->d;
		mParam[K] = parent->k;
		mParam[DETECTOR_AVG] = parent->detectorAvg;
		mParam[COMP_GAIN] = parent->compGain;
		mParam[MAX_COMP_DIFF_DB] = parent->maxCompDiffDB;
	}

	CompressorInstance::~CompressorInstance() = default;

	void CompressorInstance::filterChannel(float* aBuffer, unsigned int aSamples, float aSamplerate, SoLoud::time aTime, unsigned int aChannel, unsigned int aChannels)
	{
		auto start = high_resolution_clock::now();
		parent->Process(aSamples, aBuffer);
		auto timeElapsed = (high_resolution_clock::now() - start);
		printf("%i \n", duration_cast<nanoseconds>(timeElapsed));
	}

	//////////////////COMPRESSOR FUNCTIONS

	CompressionFilter::CompressionFilter()
	{
		//im gonna need to know more of this info later
		pregain = 0.0f;
		ratio = 12.0f;
		attack = 0.003f;
		release = 0.250f;
		preDelay = 0.0f;
		releaseZone1 = 0.09f;
		releaseZone2 = 0.160f;
		releaseZone3 = 0.420f;
		releaseZone4 = 0.980f;
		postGain = 1.0f;
		threshold = -24.0f;
		knee = 30.0f;
		wet = 1.0f;

		InitState(44100);
	}

	CompressionFilter::CompressionFilter(float sampleRate)
	{
		pregain = 0.1f;
		ratio = 3.0f;
		attack = 0.01f;
		release = 0.1f;
		preDelay = 0.0001f;
		releaseZone1 = 0.1f;
		releaseZone2 = 0.4f;
		releaseZone3 = 0.6f;
		releaseZone4 = 0.9f;
		postGain = 0.5f;

		InitState(sampleRate);
	}

	CompressionFilter::~CompressionFilter() = default;

	void CompressionFilter::updateParams(SoLoud::time aTime)
	{

	}

	int CompressionFilter::getParamCount()
	{
		return 31;
	}

	const char* CompressionFilter::getParamName(unsigned int aParamIndex)
	{
		switch (aParamIndex)
		{
		case 0:
		{
			return "meterGain";
		}

		case 1:
		{
			return "threshold";
		}

		case 2:
		{
			return "knee";
		}

		case 3:
		{
			return "linearPregain";
		}

		case 4:
		{
			return "linearThreshold";
		}

		case 5:
		{
			return "slope";
		}

		case 6:
		{
			return "attackSamplesInv";
		}

		case 7:
		{
			return "satReleaseSamplesInv";
		}

		case 8:
		{
			return "wet";
		}

		case 9:
		{
			return "dry";
		}

		case 10:
		{
			return "k";
		}

		case 11:
		{
			return "kneeDBOffset";
		}

		case 12:
		{
			return "linearThresholdKnee";
		}

		case 13:
		{
			return "masterGain";
		}

		case 14:
		{
			return "a";
		}

		case 15:
		{
			return "b";
		}

		case 16:
		{
			return "c";
		}

		case 17:
		{
			return "d";
		}

		case 18:
		{
			return "detectorAvg";
		}

		case 19:
		{
			return "compGain";
		}

		case 20:
		{
			return "maxCompDiffDB";
		}

		case 21:
		{
			return "pregain";
		}

		case 22:
		{
			return "ratio";
		}

		case 23:
		{
			return "attack";
		}

		case 24:
		{
			return "release";
		}

		case 25:
		{
			return "release zone 1";
		}

		case 26:
		{
			return "release zone 2";
		}

		case 27:
		{
			return "release zone 3";
		}

		case 28:
		{
			return "release zone 4";
		}

		case 29:
		{
			return "post gain";
		}

		default:
		{
			return nullptr;
		}
		}
	}

	unsigned int CompressionFilter::getParamType(unsigned int aParamIndex)
	{
		return FLOAT_PARAM;
	}

	float CompressionFilter::getParamMax(unsigned int aParamIndex)
	{
		switch(aParamIndex)
		{

		case WET:
		{
			return 1.0f;
		}

		case KNEE:
		{
			return 50.0f;
		}

		case THRESHOLD:
		{
			return 50.0f;
		}

		case PREGAIN:
		{
			return 1.0f;
		}

		case RATIO:
		{
			return 50.0f;
		}

		case ATTACK:
		{
			return 1.0f;
		}

		case RELEASE:
		{
			return 1.0f;
		}
		
		case RELEASEZONE1:
		{
			return 1.0f;
		}

		case RELEASEZONE2:
		{
			return 1.0f;
		}

		case RELEASEZONE3:
		{
			return 1.0f;
		}

		case RELEASEZONE4:
		{
			return 1.0f;
		}

		case POSTGAIN:
		{
			return 1.0f;
		}

		default:
		{
			return 0.0f;
		}
		}
	}

	float CompressionFilter::getParamMin(unsigned int aParamIndex)
	{
		switch (aParamIndex)
		{

		case WET:
		{
			return 0.0f;
		}

		case KNEE:
		{
			return 0.0f;
		}

		case THRESHOLD:
		{
			return -50.0f;
		}

		case PREGAIN:
		{
			return 0.0f;
		}

		case RATIO:
		{
			return -50.0f;
		}

		case ATTACK:
		{
			return 0.0f;
		}

		case RELEASE:
		{
			return 0.001f;
		}

		case RELEASEZONE1:
		{
			return 0.0f;
		}

		case RELEASEZONE2:
		{
			return 0.0f;
		}

		case RELEASEZONE3:
		{
			return 0.0f;
		}

		case RELEASEZONE4:
		{
			return 0.0f;
		}

		case POSTGAIN:
		{
			return 0.0f;
		}

		default:
		{
			return 0.0f;
		}
		}
	}

	CompressorInstance* CompressionFilter::createInstance()
	{
		return new CompressorInstance(this);
	}

	float CompressionFilter::getFilterParameter(unsigned int aAttributeId)
	{
		switch (aAttributeId)
		{
		case METER_GAIN:
		{
			return meterGain;
		}

		case METER_RELEASE:
		{

			return meterRelease;
		}

		case THRESHOLD:
		{
			return threshold;
		}

		case KNEE:
		{
			return knee;
		}

		case LINEAR_PREGAIN:
		{
			return linearPregain;
		}

		case LINEAR_THRESHOLD:
		{
			return linearThreshold;
		}

		case SLOPE:
		{
			return slope;
		}

		case ATTACK_SAMPLES_INV:
		{
			return attackSamplesInv;
		}

		case SAT_RELEASE_SAMPLES_INV:
		{
			return satReleaseSamplesInv;
		}

		case WET:
		{
			return wet;
		}

		case DRY:
		{
			return dry;
		}

		case K:
		{
			return k;
		}

		case KNEE_DB_OFFSET:
		{
			return kneeDBOffset;
		}

		case LINEAR_THRESHOLD_KNEE:
		{
			return linearThresholdKnee;
		}

		case MASTER_GAIN:
		{
			return masterGain;
		}

		case A:
		{
			return a;
		}

		case B:
		{
			return b;
		}

		case C:
		{
			return c;
		}

		case D:
		{
			return d;
		}

		case DETECTOR_AVG:
		{
			return detectorAvg;
		}

		case COMP_GAIN:
		{
			return compGain;
		}

		case MAX_COMP_DIFF_DB:
		{
			return maxCompDiffDB;
		}

		case PREGAIN:
		{
			return pregain;
		}

		case RATIO:
		{
			return ratio;
		}

		case ATTACK:
		{
			return attack;
		}

		case RELEASE:
		{
			return release;
		}

		case RELEASEZONE1:
		{
			return releaseZone1;
		}

		case RELEASEZONE2:
		{
			return releaseZone2;
		}

		case RELEASEZONE3:
		{
			return releaseZone3;
		}
		case RELEASEZONE4:
		{
			return releaseZone4;
		}

		case POSTGAIN:
		{
			return postGain;
		}

		default:
		{
			return 0.0f;
		}
		}
	}

	void CompressionFilter::setFilterParameter(unsigned int aAttributeId, float aValue)
	{

	}

	void CompressionFilter::fadeFilterParameter(unsigned int aAttributeId, float aTo, SoLoud::time aTime, SoLoud::time aStartTime)
	{
		//lerp? also how is this meant to be implemented?
		setFilterParameter(aAttributeId, glm::lerp<float>((float)aStartTime, aTo, (float)aTime));
	}

	void CompressionFilter::oscillateFilterParameter(unsigned int aAttributeId, float aFrom, float aTo, SoLoud::time aTime, SoLoud::time aStartTime)
	{
		//another lerp?
	}

	void CompressionFilter::InitState(float rate)
	{
		this->pregain = pregain;

		int delayBufferSize = (int)(rate * preDelay);
		if (delayBufferSize < 1)
		{
			delayBufferSize = 1;
		}
		if(delayBuffer.empty())
		{
			for (size_t iter = 0; iter < delayBufferSize; iter++)
			{
				delayBuffer.push_back(0.0f);
			}
		}

		float linearPregain = DB2lin(pregain);
		float linearThreshold = DB2lin(threshold);
		float slope = 1.0f / ratio;
		float attackSamples = rate * attack;
		float attackSamplesInv = 1.0f / attackSamples;
		float releaseSamples = rate * release;
		float satRelease = 0.0025f;
		float satReleaseSamplesInv = 1.0f / ((float)rate * satRelease);
		float dry = 1.0f - wet;

		float meterGain = 1.0f;
		float meterFalloff = 0.325f;
		float meterRelease = 1.0f - expf(-1.0f / ((float)rate * meterFalloff));

		float k = 2.0f;
		float kneeDBOffset = 0.0f;
		float linearThresholdKnee = 0.0f;
		if (knee > 0.0f)
		{
			float xKnee = DB2lin(threshold + knee);
			float minK = 0.1f;
			float maxK = 10000.0f;
			for (int iter = 0; iter < 15; iter++)
			{
				if (KneeSlope(xKnee, k, linearThreshold) < slope)
				{
					maxK = k;
				}
				else
				{
					minK = k;
				}
				k = sqrtf(minK * maxK);
			}
			kneeDBOffset = Lin2db(KneeCurve(xKnee, k, linearThreshold));
			linearThresholdKnee = DB2lin(threshold + knee);
		}

		float fullLevel = ComputeCurve(1.0f, k, slope, linearThreshold, linearThresholdKnee,
			threshold, knee, kneeDBOffset);
		float masterGain = DB2lin(postGain) * powf(1.0f / fullLevel, 0.6f);

		// calculate the adaptive release curve parameters
		// solve a,b,c,d in `y = a*x^3 + b*x^2 + c*x + d`
		// intersecting points (0, y1), (1, y2), (2, y3), (3, y4)
		float y1 = releaseSamples * releaseZone1;
		float y2 = releaseSamples * releaseZone2;
		float y3 = releaseSamples * releaseZone3;
		float y4 = releaseSamples * releaseZone4;
		float a = (-y1 + 3.0f * y2 - 3.0f * y3 + y4) / 6.0f;
		float b = y1 - 2.5f * y2 + 2.0f * y3 - 0.5f * y4;
		float c = (-11.0f * y1 + 18.0f * y2 - 9.0f * y3 + 2.0f * y4) / 6.0f;
		float d = y1;

		this->meterGain = 1.0f;
		this->meterRelease = meterRelease;
		this->threshold = threshold;
		this->knee = knee;
		this->wet = wet;
		this->linearPregain = linearPregain;
		this->linearThreshold = linearThreshold;
		this->slope = slope;
		this->attackSamplesInv = attackSamplesInv;
		this->satReleaseSamplesInv = satReleaseSamplesInv;
		this->dry = dry;
		this->k = k;
		this->kneeDBOffset = kneeDBOffset;
		this->linearThresholdKnee = linearThresholdKnee;
		this->masterGain = masterGain;
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->detectorAvg = 0.0f;
		this->compGain = 1.0f;
		this->maxCompDiffDB = -1.0f;
		this->delayBufSize = delayBufferSize;
		this->delayWritePos = 0;
		this->delayReadPos = delayBufSize > 1 ? 1 : 0;
	}

	float CompressionFilter::DB2lin(float db)
	{
		return powf(10.0f, 0.05f * db);
	}

	float CompressionFilter::Lin2db(float lin)
	{
		return 20.0f * log10f(lin);
	}

	float CompressionFilter::KneeCurve(float x, float k, float linearThreshold)
	{
		return linearThreshold + (1.0f - expf(-k * (x - linearThreshold))) / k;
	}

	float CompressionFilter::KneeSlope(float x, float k, float linearThreshold)
	{
		return k * x / ((k * linearThreshold + 1.0f) * expf(k * (x - linearThreshold)) - 1);
	}

	float CompressionFilter::ComputeCurve(float x, float k, float slope, float linearThreshold, float linearThresholdKnee, float threshold, float knee, float kneeBDOffset)
	{
		if (x < linearThreshold)
		{
			return x;
		}

		if (knee <= 0.0f)
		{
			return DB2lin(threshold + slope * (Lin2db(x) - threshold));
		}

		if (x < linearThresholdKnee)
		{
			return KneeCurve(x, k, linearThreshold);
		}
		return DB2lin(kneeBDOffset + slope * (Lin2db(x) - threshold - knee));
	}

	float CompressionFilter::AdaptiveReleaseCurve(float x, float a, float b, float c, float d)
	{
		float x2 = x * x;
		return a * x2 * x + b * x2 + c * x + d;
	}

	float CompressionFilter::Fixf(float v, float def)
	{
		if (isnan(v) || isinf(v))
		{
			return def;
		}
		return v;
	}

	void CompressionFilter::Process(int size, float* input)
	{
		float meterGain = this->meterGain;
		float meterRelease = this->meterRelease;
		float threshold = this->threshold;
		float knee = this->knee;
		float linearPregain = this->linearPregain;
		float linearThreshold = this->linearThreshold;
		float slope = this->slope;
		float attackSamplesInv = this->attackSamplesInv;
		float satReleasesamplesInv = this->satReleaseSamplesInv;
		float wet = this->wet;
		float dry = this->dry;
		float k = this->k;
		float kneeDBOffset = this->kneeDBOffset;
		float linearThresholdKnee = this->linearThresholdKnee;
		float masterGain = this->masterGain;
		float a = this->a;
		float b = this->b;
		float c = this->c;
		float d = this->d;
		float detectorAvg = this->detectorAvg;
		float compGain = this->compGain;
		float maxCompDiffDB = this->maxCompDiffDB;
		int delayBufSize = this->delayBufSize;
		int delayWritePos = this->delayWritePos;
		int delayReadPos = this->delayReadPos;

		int samplesPerChunk = SAMPLES_PER_UPDATE;
		int chunks = size / samplesPerChunk;
		float ang90 = (float)M_PI * 0.5f;
		float ang90inv = 2.0f / (float)M_PI;
		int samplePos = 0;
		float spacingDB = SPACINGDB;

		for (int ch = 0; ch < chunks; ch++)
		{
			detectorAvg = Fixf(detectorAvg, 1.0f);
			float desiredGain = detectorAvg;
			float scaledDesiredGain = asinf(desiredGain) * ang90inv;
			float compDiffDB = Lin2db(compGain / scaledDesiredGain);

			float enveloperate;
			if (compDiffDB < 0.0f)
			{
				compDiffDB = Fixf(compDiffDB, -1.0f);
				maxCompDiffDB = -1;

				float x = (glm::clamp(compDiffDB, -12.0f, 0.0f) + 12.0f) * 0.25f;
				float releaseSamples = AdaptiveReleaseCurve(x, a, b, c, d);
				enveloperate = DB2lin(spacingDB / releaseSamples);
			}

			else
			{
				compDiffDB = Fixf(compDiffDB, 1.0f);
				if (maxCompDiffDB == -1 || maxCompDiffDB < compDiffDB)
				{
					maxCompDiffDB = compDiffDB;
				}
				float attenuate = maxCompDiffDB;
				if (attenuate < 0.5f)
				{
					attenuate = 0.5f;
				}
				enveloperate = 1.0f - powf(0.25f / attenuate, attackSamplesInv);
			}

			for (int chi = 0; chi < samplesPerChunk; chi++, samplePos++,
				delayReadPos = (delayReadPos + 1) % delayBufSize,
				delayWritePos = (delayWritePos + 1) % delayBufSize)
			{
				float samp = input[samplePos] * linearPregain;
				delayBuffer[delayWritePos] = samp;

				float inputMax = abs(samp);

				float attenuation;
				if (inputMax < 0.0001f)
				{
					attenuation = 1.0f;
				}
				else
				{
					float inputComp = ComputeCurve(inputMax, k, slope, linearThreshold,
						linearThresholdKnee, threshold, knee, kneeDBOffset);
					attenuation = inputComp / inputMax;
				}

				float rate;
				if (attenuation > detectorAvg)
				{
					float attenuationDB = -Lin2db(attenuation);
					if (attenuationDB < 2.0f)
					{
						attenuationDB = 2.0f;
					}
					float DBPerSample = attenuationDB * satReleaseSamplesInv;
					rate = DB2lin(DBPerSample) -1.0f;
				}
				else
				{
					rate = 1.0f;
				}

				detectorAvg += (attenuation - detectorAvg) * rate;
				if (detectorAvg > 1.0f)
				{
					detectorAvg = 1.0f;
				}
				detectorAvg = Fixf(detectorAvg, 1.0f);

				if (enveloperate < 1)
				{
					compGain += (scaledDesiredGain - compGain) * enveloperate;
				}
				else
				{
					compGain *= enveloperate;
					if (compGain > 1.0f)
					{
						compGain = 1.0f;
					}
				}

				float premixGain = sinf(ang90 * compGain);
				float gain = dry + wet * masterGain * premixGain;

				float premixGainDB = Lin2db(premixGain);
				if (premixGainDB < meterGain)
				{
					meterGain = premixGainDB;
				}
				else
				{
					meterGain += (premixGainDB - meterGain) * meterRelease;
				}
				input[samplePos] = delayBuffer[delayReadPos] * gain;
			}
		}

		this->meterGain = meterGain;
		this->detectorAvg = detectorAvg;
		this->compGain = compGain;
		this->maxCompDiffDB = maxCompDiffDB;
		this->delayWritePos = delayWritePos;
		this->delayReadPos = delayReadPos;
	}
}