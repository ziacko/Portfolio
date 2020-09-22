#pragma once

#define MAX_DELAY_BUFFER_SIZE 1024
#define SAMPLES_PER_UPDATE 32
#define SPACINGDB 5.0f

#include <soloud.h>
#include <soloud_speech.h>
#include <soloud_openmpt.h>
#include <soloud_wav.h>
#include <soloud_thread.h>
#include <soloud_freeverbfilter.h>
#include <soloud_biquadresonantfilter.h>
#include <soloud_bassboostfilter.h>
#include <soloud_eqfilter.h>
#include <vector>

//https://github.com/voidqk/sndfilter
namespace SoLoud
{
	class CompressionFilter;

	enum filterParams_t
	{
		METER_GAIN = 0,
		METER_RELEASE,
		THRESHOLD,
		KNEE,
		LINEAR_PREGAIN,
		LINEAR_THRESHOLD,
		SLOPE,
		ATTACK_SAMPLES_INV,
		SAT_RELEASE_SAMPLES_INV,
		WET,
		DRY,
		KNEE_DB_OFFSET,
		LINEAR_THRESHOLD_KNEE,
		MASTER_GAIN,
		A,
		B,
		C,
		D,
		K,
		DETECTOR_AVG,
		COMP_GAIN,
		MAX_COMP_DIFF_DB,
		PREGAIN,
		RATIO,
		ATTACK,
		RELEASE,
		RELEASEZONE1,
		RELEASEZONE2,
		RELEASEZONE3,
		RELEASEZONE4,
		POSTGAIN
	};

	class CompressorInstance : public SoLoud::FilterInstance
	{
	public:
		CompressorInstance(CompressionFilter* parent);
		virtual ~CompressorInstance();

		void filterChannel(float* aBuffer, unsigned int aSamples, float aSamplerate, SoLoud::time aTime, unsigned int aChannel, unsigned int aChannels) override;

		CompressionFilter* parent;
	};

	class CompressionFilter : public SoLoud::Filter
	{
	public:
		CompressionFilter();
		CompressionFilter(float sampleRate);
		virtual ~CompressionFilter();

		CompressorInstance* createInstance() override;

		int getParamCount() override;
		const char* getParamName(unsigned int aParamIndex) override;
		unsigned int getParamType(unsigned int aParamIndex) override;
		float getParamMax(unsigned int aParamIndex) override;
		float getParamMin(unsigned int aParamIndex) override;

		virtual void updateParams(SoLoud::time aTime);

		virtual float getFilterParameter(unsigned int aAttributeId);

		virtual void setFilterParameter(unsigned int aAttributeId, float aValue);

		virtual void fadeFilterParameter(unsigned int aAttributeId, float aTo, SoLoud::time aTime, SoLoud::time aStartTime);

		virtual void oscillateFilterParameter(unsigned int aAttributeId, float aFrom, float aTo, SoLoud::time aTime, SoLoud::time aStartTime);

		void InitState(float rate);

		inline float DB2lin(float db);

		inline float Lin2db(float lin);

		inline float KneeCurve(float x, float k, float linearThreshold);

		inline float KneeSlope(float x, float k, float linearThreshold);

		inline float ComputeCurve(float x, float k, float slope, float linearThreshold,
			float linearThresholdKnee, float threshold, float knee, float kneeBDOffset);

		inline float AdaptiveReleaseCurve(float x, float a, float b, float c, float d);

		inline float Fixf(float v, float def);

		void Process(int size, float* input);

		// user can read the metergain state variable after processing a chunk to see how much dB the
		// compressor would have liked to compress the sample; the meter values aren't used to shape the
		// sound in any way, only used for output if desired
		float meterGain;
		float meterRelease;
		float threshold;
		float knee;
		float linearPregain;
		float linearThreshold;
		float slope;
		float attackSamplesInv;
		float satReleaseSamplesInv;
		float wet;
		float dry;
		float k;
		float kneeDBOffset;
		float linearThresholdKnee;
		float masterGain;
		float a; // adaptive release polynomial coefficients
		float b;
		float c;
		float d;
		float detectorAvg;
		float compGain;
		float maxCompDiffDB;
		float pregain;
		float ratio;
		float attack;
		float release;
		float preDelay;
		float releaseZone1;
		float releaseZone2;
		float releaseZone3;
		float releaseZone4;
		float postGain;
		int delayBufSize;
		int delayWritePos;
		int delayReadPos;

		std::vector<float> delayBuffer;
	};
}
