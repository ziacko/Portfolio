#ifndef GAMMA_H
#define GAMMA_H
#include <TexturedScene.h>

struct gammaSettings_t
{
	glm::vec3		gammaSettings;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	gammaSettings_t(glm::vec3 gammaSettings = glm::vec3(1.2, 3.7, 2.0))
	{
		this->gammaSettings = gammaSettings;
	}

	~gammaSettings_t(){};
};

class gammaScene : public texturedScene
{
public:

	gammaScene(
		gammaSettings_t* gammaSettings = new gammaSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (gamma)",
		camera* gammaCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/gamma.txt")
		: texturedScene(defaultTexture, windowName, gammaCamera, shaderConfigPath)
	{
		this->gammaSettings = gammaSettings;
	}

	~gammaScene(){};

protected:

	gammaSettings_t*		gammaSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		ImGui::SliderFloat("gamma red", &gammaSettings->gammaSettings.r, 0.f, 10.0f);
		ImGui::SliderFloat("gamma green", &gammaSettings->gammaSettings.g, 0.0f, 10.0f);
		ImGui::SliderFloat("gamma blue", &gammaSettings->gammaSettings.b, 0.0f, 10.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<gammaSettings_t>(gammaSettings, gammaSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<gammaSettings_t>(gammaSettings, gammaSettings->bufferHandle);
	}
};

#endif