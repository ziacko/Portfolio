#ifndef SOUND_TEST_H
#define SOUND_TEST_H
#include <Scene.h>
#include <soloud.h>
#include <soloud_speech.h>
#include <soloud_openmpt.h>
#include <soloud_wav.h>
#include <soloud_thread.h>

class gamepadScene : public scene
{
public:

	gamepadScene(
		const char* windowName = "Ziyad Barakat's portfolio (sound test)",
		camera* erosionCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Default.txt")
		: scene(windowName, erosionCamera, shaderConfigPath)
	{
		gpad = nullptr;
	}

protected:

	gamepad_t* gpad;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{

		static bool blarg = true;
		scene::BuildGUI(windows[0], io);

		ImGui::ShowMetricsWindow();

		ImGui::Begin("gamepad", &blarg, ImGuiWindowFlags_MenuBar);

		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("Open..")) { }
				ImGui::EndMenu();
			}
		}

		ImGui::EndMenuBar();
		
		ImGui::ProgressBar(gpad->rightTrigger, ImVec2(-1, 0), "right trigger");
		ImGui::ProgressBar(gpad->leftTrigger, ImVec2(-1, 0), "left trigger");	

		if(ImGui::TreeNode("axes"))
		{
			if(ImGui::TreeNode("right"))
			{
				ImGui::Text("X %f", gpad->rightStick[0]);
				ImGui::SameLine();
				ImGui::Text("Y %f", gpad->rightStick[1]);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("left"))
			{
				ImGui::Text("X %f", gpad->leftStick[0]);
				ImGui::SameLine();
				ImGui::Text("Y %f", gpad->leftStick[1]);
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if(ImGui::TreeNode("buttons"))
		{
			if(ImGui::TreeNode("face"))
			{
				ImGui::Text("top: %s", (gpad->buttonStates[gamepad_t::button_t::face_top]) ? "down" : "up");
				ImGui::Text("bottom: %s", (gpad->buttonStates[gamepad_t::button_t::face_bottom]) ? "down" : "up");
				ImGui::Text("left: %s", (gpad->buttonStates[gamepad_t::button_t::face_left]) ? "down" : "up");
				ImGui::Text("right: %s", (gpad->buttonStates[gamepad_t::button_t::face_right]) ? "down" : "up");
				ImGui::TreePop();
			}

			if(ImGui::TreeNode("D-pad"))
			{
				ImGui::Text("top: %s", (gpad->buttonStates[gamepad_t::button_t::Dpad_top]) ? "down" : "up");
				ImGui::Text("down: %s", (gpad->buttonStates[gamepad_t::button_t::Dpad_bottom]) ? "down" : "up");
				ImGui::Text("left: %s", (gpad->buttonStates[gamepad_t::button_t::Dpad_left]) ? "down" : "up");
				ImGui::Text("right: %s", (gpad->buttonStates[gamepad_t::button_t::Dpad_right]) ? "down" : "up");
				ImGui::TreePop();
			}

			if(ImGui::TreeNode("shoulders"))
			{
				ImGui::Text("left: %s", (gpad->buttonStates[gamepad_t::button_t::left_shoulder]) ? "down" : "up");
				ImGui::Text("right: %s", (gpad->buttonStates[gamepad_t::button_t::right_shoulder]) ? "down" : "up");
				ImGui::TreePop();
			}

			if(ImGui::TreeNode("options"))
			{
				ImGui::Text("start: %s", (gpad->buttonStates[gamepad_t::button_t::start]) ? "down" : "up");
				ImGui::Text("select: %s", (gpad->buttonStates[gamepad_t::button_t::select]) ? "down" : "up");
				ImGui::TreePop();
			}

			if(ImGui::TreeNode("sticks"))
			{
				ImGui::Text("left: %s", (gpad->buttonStates[gamepad_t::button_t::left_stick]) ? "down" : "up");
				ImGui::Text("right: %s", (gpad->buttonStates[gamepad_t::button_t::right_stick]) ? "down" : "up");
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

		ImGui::ShowDemoWindow();

		ImGui::End();
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
	}

	void Update() override
	{
		scene::Update();
		gpad = manager->GetGamepads()[0];
	}
};

#endif