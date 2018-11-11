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
		const char* shaderConfigPath = "../../resources/shaders/Contrast.txt")
		: texturedScene(defaultTexture, windowName, contrastCamera, shaderConfigPath)
	{
		this->contrastSettings = contrastSettings;
	}

	~contrastScene(){};

protected:

	contrastSettings_t*			contrastSettings;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::SliderFloat("contrast level", &contrastSettings->contrast, 0.0f, 10.0f);
	}


	void InitializeBuffers() override
	{
		scene::InitializeUniforms();
		SetupBuffer(contrastSettings, contrastSettings->bufferHandle, sizeof(*contrastSettings), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(contrastSettings, contrastSettings->bufferHandle, sizeof(*contrastSettings), gl_uniform_buffer, gl_dynamic_draw);
	}

};

#endif
