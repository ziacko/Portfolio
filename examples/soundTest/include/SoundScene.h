#ifndef SOUND_TEST_H
#define SOUND_TEST_H
#include <Scene.h>
#include "Compressor.h"
#include "soloud_lofifilter.h"

class soundScene : public scene
{
public:

	soundScene(
		const char* windowName = "Ziyad Barakat's portfolio (sound test)",
		camera* erosionCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Default.txt",
		const char* soundPath = "../../resources/audio/music/Johny (Moon Boots Remix).ogg")
		: scene(windowName, erosionCamera, shaderConfigPath)
	{
		soundSys.init();
		soundSource.load(soundPath);
		//speechTest.
		speechTest.setText("mortis");
		int x = reverbFilter.getParamCount();
		for (size_t iter = 0; iter < x; iter++)
		{
			printf("%s \n", reverbFilter.getParamName(iter));
		}

		//do we pass in filters or do we use ID as filter index internally?
		testBus.setFilter(0, &compressorFilter);
		//testBus.setFilter(1, &bqdResFilter);
		
		busHandle = soundSys.play(testBus);
		//busHandle2 = testBus.play(testBus2);

		//routeHandle = testBus.play(testBus2);
		soundHandle = testBus.play(soundSource);
	}

	SoLoud::Soloud soundSys;
	SoLoud::Wav soundSource;
	SoLoud::Speech speechTest;
	int soundHandle;
	int routeHandle;
	//SoLoud::WavInstance* soundInstance;
	std::string soundPath;
	SoLoud::Bus testBus;
	SoLoud::Bus testBus2;

	int busHandle;
	int busHandle2;

	SoLoud::FreeverbFilter reverbFilter;
	SoLoud::BiquadResonantFilter bqdResFilter;
	SoLoud::BassboostFilter bassFilter;

	SoLoud::CompressionFilter compressorFilter;

protected:

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene::BuildGUI(windows[0], io);

		BuildAudioGUI();
		BuildCompressorSettings();
	}

	void BuildAudioGUI()
	{
		unsigned int channels = soundSys.getBackendChannels();

		ImGui::Begin("Soloud");
		ImGui::Text("version# %i", soundSys.getVersion());

		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "sound system");
		ImGui::Text("active voice count: %i", soundSys.getActiveVoiceCount());
		ImGui::Text("num backend channels %i", channels);
		for (size_t iter = 0; iter < channels; iter++)
		{
			float x, y, z = 0;
			ImGui::Text("channel %i volume: %f", iter, soundSys.getApproximateVolume(iter));
			soundSys.getSpeakerPosition(iter, x, y, z);
			ImGui::Text("channel %i position: (%0.f:x, %0.f:y, %0.f:z)", iter, x, y, z);
		}
		ImGui::Text("backend buffer size: %i (bytes?)", soundSys.getBackendBufferSize());
		ImGui::Text("backend: %s", soundSys.getBackendString());
		ImGui::Text("backend sample rate %i", soundSys.getBackendSamplerate());
		ImGui::Text("global volume: %f", soundSys.getGlobalVolume());
		ImGui::Text("max active voice count: %i", soundSys.getMaxActiveVoiceCount());
		ImGui::Text("post clip scaler: %i", soundSys.getPostClipScaler());

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "sound source");
		ImGui::Text("SoLoud ID %i", soundSource.mAudioSourceID);
		ImGui::Text("sound length: %.1f seconds", soundSource.getLength());
		ImGui::Text("loop point: %.1f seconds", soundSource.getLoopPoint());
		ImGui::Text("sample rate: %.1f", soundSource.mBaseSamplerate);
		ImGui::Text("#channels: %i", soundSource.mChannels);
		ImGui::Text("sample count: %i", soundSource.mSampleCount);
		ImGui::Text("default volume: %f", soundSource.mVolume);

		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "sound instance");
		ImGui::Text("play index: %i", soundSys.mPlayIndex);

		//ImGui::SameLine();
		if (ImGui::Button("Play"))
		{
			//soundHandle = soundSys.play(soundSource);
		}
		static bool paused = false;
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			paused = !paused;
			soundSys.setPause(soundHandle, paused);
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop"))
		{
			soundSys.stop(soundHandle);
		}

		//ImGui::Text("instance ID %i", soundInstance->mAudioSourceID);
		static float sampleRate = soundSource.mBaseSamplerate;
		if (ImGui::DragFloat("sample rate", &sampleRate))
		{
			soundSys.setSamplerate(soundHandle, sampleRate);
		}

		ImGui::Text("loop count %i", soundSys.getLoopCount(soundHandle));
		//ImGui::Text("is looping %i", soundSys.getLooping(soundHandle));
		static bool looping = false;
		if (ImGui::Checkbox("looping", &looping))
		{
			soundSys.setLooping(soundHandle, looping);
		}

		static float loopPoint = 0.0f;
		if (ImGui::DragFloat("loop point: ", &loopPoint, 1.0f, 0.0f, soundSource.getLength()))
		{
			soundSys.setLoopPoint(soundHandle, loopPoint);
		}

		ImGui::Text("overall volume: %i", soundSys.getVolume(soundHandle));

		static float pan = 0.0f;
		if (ImGui::DragFloat("pan", &pan, 0.01f, -1.0f, 1.0f))
		{
			soundSys.setPan(soundHandle, pan);
		}

		float streamPosition = soundSys.getStreamPosition(soundHandle);
		if (ImGui::DragFloat("stream position: ", &streamPosition, 0.01f, 0.0f, soundSource.getLength()))
		{
			soundSys.seek(soundHandle, streamPosition);
		}
		ImGui::Text("stream time: %f", soundSys.getStreamTime(soundHandle));

		bool autoStop = soundSys.getAutoStop(soundHandle);
		if (ImGui::Checkbox("auto stop", &autoStop))
		{
			soundSys.setAutoStop(soundHandle, autoStop);
		}

		static float volume = soundSys.getVolume(soundHandle);
		if (ImGui::DragFloat("volume:", &volume, 0.01f, 0.0f, 1.0f))
		{
			soundSys.setVolume(soundHandle, volume);
		}

		static float relativePlaySpeed = soundSys.getRelativePlaySpeed(soundHandle);
		if (ImGui::DragFloat("relative play speed", &relativePlaySpeed, 0.01f, 0.01f, 10.0f))
		{
			soundSys.setRelativePlaySpeed(soundHandle, relativePlaySpeed);
		}

		ImGui::End();

	}

	void BuildCompressorSettings()
	{
		ImGui::Begin("compression filter");

		if(ImGui::DragFloat("wet: ", &compressorFilter.wet, 0.01f, 0.0f, 1.0f))
		{
			//compressorFilter.dry = 1.0f - compressorFilter.wet;
		}
		ImGui::DragFloat("knee: ", &compressorFilter.knee, 0.01f, 0.0f, 50.0f);
		ImGui::DragFloat("threshold: ", &compressorFilter.threshold, 0.01f, -50.0f, 50.0f);
		ImGui::DragFloat("pregain: ", &compressorFilter.pregain, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("ratio: ", &compressorFilter.ratio, 1.0f, 0.1f, 50.0f);
		ImGui::DragFloat("attack: ", &compressorFilter.attack, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("release: ", &compressorFilter.release, 0.01f, 0.001f, 1.0f);
		ImGui::DragFloat("release zone 1: ", &compressorFilter.releaseZone1, 0.01f, 0.001f, 1.0f);
		ImGui::DragFloat("release zone 2: ", &compressorFilter.releaseZone2, 0.01f, 0.001f, 1.0f);
		ImGui::DragFloat("release zone 3: ", &compressorFilter.releaseZone3, 0.01f, 0.001f, 1.0f);
		ImGui::DragFloat("release zone 4: ", &compressorFilter.releaseZone4, 0.01f, 0.001f, 1.0f);
		ImGui::DragFloat("post gain: ", &compressorFilter.postGain, 0.01f, 0.0f, 1.0f);

		if(ImGui::Button("reset filter"))
		{
			compressorFilter.InitState(soundSource.mBaseSamplerate);
		}	

		//read only data
		ImGui::Text("detectorAvg: %f", compressorFilter.detectorAvg);
		ImGui::Text("comp gain: %f", compressorFilter.compGain);
		ImGui::Text("max comp diff DB: %f", compressorFilter.maxCompDiffDB);
		ImGui::Text("linear Threshold knee: %f", compressorFilter.linearThresholdKnee);
		ImGui::Text("knee DB Offset: %f", compressorFilter.kneeDBOffset);
		ImGui::Text("k: %f", compressorFilter.k);
		ImGui::Text("attack samples Inv: %f", compressorFilter.attackSamplesInv);
		ImGui::Text("linear threshold: %f", compressorFilter.linearThreshold);
		ImGui::Text("linear pregain: %f", compressorFilter.linearPregain);		
		ImGui::Text("slope: %f", compressorFilter.slope);
		ImGui::Text("sat release samples Inv: %f", compressorFilter.satReleaseSamplesInv);
		ImGui::Text("master gain: %f", compressorFilter.masterGain);
		ImGui::Text("meter gain: %f", compressorFilter.meterGain);
		ImGui::Text("meter release: %f", compressorFilter.meterRelease);
		ImGui::Text("a: %f", compressorFilter.a);
		ImGui::Text("b: %f", compressorFilter.b);
		ImGui::Text("c: %f", compressorFilter.c);
		ImGui::Text("d: %f", compressorFilter.d);

		ImGui::End();
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
	}

	void Update() override
	{
		scene::Update();
	}
};

#endif