#ifndef PROFILER_H
#define PROFILER_H

#define MICROPROFILE_IMPL
#define MICROPROFILE_MAX_FRAME_HISTORY (2<<10)
#define MICROPROFILE_CONTEXT_SWITCH_TRACE 1
#define MICROPROFILE_GPU_TIMERS_GL 1
#define GL_TIMESTAMP 1

#include "../../examples/TemporalAA/include/TemporalAA.h"
#include "microprofile.h"

MICROPROFILE_DEFINE(MAIN, "MAIN", "CPU", 0xff0000);

class profiler : public temporalAA
{
public:

	profiler(
		const char* windowName = "Ziyad Barakat's portfolio (temporal AA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/TemporalAA.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: temporalAA(windowName, texModelCamera, shaderConfigPath, model)
	{

	}

	virtual ~profiler() {};

	void Initialize() override
	{
		temporalAA::Initialize();
		MicroProfileSetForceEnable(true);
		MicroProfileSetEnableAllGroups(true);
		MicroProfileSetForceMetaCounters(true);
		MicroProfileWebServerStart();
		MicroProfileContextSwitchTraceStart();
		MicroProfileGpuInitGL();


		//MICROPROFILE_COUNTER_SET("GPU memory//default framebuffer stats", 0);
		/*MICROPROFILE_COUNTER_ADD("memory/main", 1000);
		MICROPROFILE_COUNTER_ADD("memory/gpu/vertexbuffers", 1000);
		MICROPROFILE_COUNTER_ADD("memory/gpu/indexbuffers", 200);
		MICROPROFILE_COUNTER_ADD("memory//main", 1000);
		MICROPROFILE_COUNTER_ADD("memory//", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//mainx/\\//", 1000);
		MICROPROFILE_COUNTER_ADD("//memoryx//mainx/", 1000);
		MICROPROFILE_COUNTER_ADD("//memoryy//main/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main0/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main1/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main2/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main3/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main4/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main5/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main6/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main7/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main8/", 1000);
		MICROPROFILE_COUNTER_ADD("//memory//main9/", 1000);
		MICROPROFILE_COUNTER_ADD("//\\\\///lala////lelel", 1000);*/


		//MICROPROFILE_COUNTER_CONFIG("/test/sinus", MICROPROFILE_COUNTER_FORMAT_BYTES, 0, MICROPROFILE_COUNTER_FLAG_DETAILED));
		//MICROPROFILE_COUNTER_CONFIG("/test/cosinus", MICROPROFILE_COUNTER_FORMAT_DEFAULT, 0, MICROPROFILE_COUNTER_FLAG_DETAILED));
		//MICROPROFILE_COUNTER_CONFIG("/runtime/tw_frame_events", MICROPROFILE_COUNTER_FORMAT_DEFAULT, 0, MICROPROFILE_COUNTER_FLAG_DETAILED);
	}

protected:

	void Update() override
	{
		temporalAA::Update();
		MICROPROFILE_SCOPE(MAIN);
		MICROPROFILE_COUNTER_ADD("engine/frames", 1);

	}

	void UpdateDefaultBuffer()
	{
		temporalAA::UpdateDefaultBuffer();
	}

	void Draw() override
	{
		MICROPROFILE_SCOPEI("CPU", "Render", 0);
		temporalAA::Draw();
		MicroProfileFlip();
	}

	virtual void JitterPass() override
	{
		MICROPROFILE_SCOPEGPUI("Jitter pass", 0xff00ff);
		MICROPROFILE_SCOPEI("CPU", "Jitter pass", 0xff00ff);
		temporalAA::JitterPass();
	}

	virtual void UnJitteredPass() override
	{
		//MICROPROFILE_SCOPEGPUI("Unjittered pass", 0xff00ff);
		//MICROPROFILE_SCOPEI("CPU", "Unjittered pass", 0xff00ff);
		temporalAA::UnJitteredPass();
	}

	virtual void TAAPass() override
	{
		MICROPROFILE_SCOPEGPUI("Temporal AA pass", 0xff00ff);
		MICROPROFILE_SCOPEI("CPU", "Temporal AA pass", 0xff00ff);
		temporalAA::TAAPass();
	}

	void FinalPass(texture* tex1, texture* tex2) override
	{
		MICROPROFILE_SCOPEGPUI("Final pass", 0xff00ff);
		MICROPROFILE_SCOPEI("CPU", "Final pass", 0xff00ff);
		temporalAA::FinalPass(tex1, tex2);
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		MICROPROFILE_SCOPEI("CPU", "building ImGUI", 0xff00ff);
		temporalAA::BuildGUI(windows[0], io);
	}

	void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state) override
	{
		temporalAA::HandleMouseClick(window, button, state);
		bool leftMouseButton = (button == mouseButton_t::left) && (state == buttonState_t::down);
		bool rightMouseButton = (button == mouseButton_t::right) && (state == buttonState_t::down);
		//MicroPr(leftMouseButton, rightMouseButton);

	}

	void HandleMouseMotion(tWindow* window, vec2_t<int> windowPosition, vec2_t<int> screenPosition) override
	{
		temporalAA::HandleMouseMotion(window, windowPosition, screenPosition);
		//MicroProfileMousePosition(windowPosition.x, windowPosition.y, 0);
	}
};

#endif