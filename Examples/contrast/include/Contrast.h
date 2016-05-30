#ifndef CONTRAST_H
#define CONTRAST_H

#include <TexturedScene.h>

struct contrastSettings_t
{
	float			contrast;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	contrastSettings_t(GLfloat contrast = 1.5f)
	{
		this->contrast = contrast;
	}

	~contrastSettings_t(){};
};

class contrastScene : public texturedScene
{
public:

	contrastScene(
		contrastSettings_t* contrastSettings = new contrastSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (contrast)",
		camera* contrastCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/contrast.txt")
		: texturedScene(defaultTexture, windowName, contrastCamera, shaderConfigPath)
	{
		this->contrastSettings = contrastSettings;
	}

	~contrastScene(){};

protected:

	contrastSettings_t*			contrastSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		ImGui::SliderFloat("contrast level", &contrastSettings->contrast, -1.0f, 10.0f);
	}


	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<contrastSettings_t>(contrastSettings, contrastSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<contrastSettings_t>(contrastSettings, contrastSettings->bufferHandle);
	}

};

#endif