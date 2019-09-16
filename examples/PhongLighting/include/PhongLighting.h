#ifndef PHONGLIGHTING_H
#define PHONGLIGHTING_H
#include "../../texturedModel/include/texturedModel.h"
#include <Light.h>

class PhongLighting : public texturedModel
{
public:

	PhongLighting(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 1.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/PhongLighting.txt",
		model_t* model = new model_t("../../resources/models/Sponza/Sponza.fbx"))
		: texturedModel(windowName, texModelCamera, shaderConfigPath, model)
	{
		defaultLight = new light();
	}

	~PhongLighting(){};

	virtual void Initialize() override
	{
		texturedModel::Initialize();
	}

protected:

	light* defaultLight;

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedModel::BuildGUI(window, io);
		LightGUI();
	}

	virtual void LightGUI()
	{
		ImGui::Begin("Light Settings");
		ImGui::ListBox("type", &defaultLight->currentType, defaultLight->lightTypes.data(), defaultLight->lightTypes.size());
		ImGui::Separator();
		ImGui::ColorPicker4("color", &defaultLight->color.x);
		ImGui::Separator();
		ImGui::DragFloat4("direction", &defaultLight->direction.x);
		ImGui::DragFloat4("position", &defaultLight->position.x);
		ImGui::Separator();
		ImGui::DragFloat("intensity", &defaultLight->intensity);
		if (defaultLight->currentType == point || defaultLight->currentType == spot)
		{
			ImGui::DragFloat("range", &defaultLight->range);
		}
		if (defaultLight->currentType == spot)
		{
			ImGui::DragFloat("spot angle", &defaultLight->angle);
		}
		
		ImGui::End();
	}

	virtual void Draw()
	{
		DrawMeshes();
	
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void Update() override
	{
		texturedModel::Update();
		UpdateBuffer(defaultLight, defaultLight->bufferHandle, sizeof(*defaultLight), gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void InitializeUniforms() override
	{
		texturedModel::InitializeUniforms();
		SetupBuffer(defaultLight, defaultLight->bufferHandle, sizeof(*defaultLight), 2, gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif