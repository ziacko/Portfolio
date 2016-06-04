#ifndef EROSION_H
#define EROSION_H
#include <TexturedScene.h>

struct erosionSettings_t
{
	float			strengthX;
	float			strengthY;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	erosionSettings_t(
		GLfloat strengthX = 1.0f, GLfloat strengthY= 1.0f)
	{
		this->strengthX = strengthX;
		this->strengthY = strengthY;
	}

	~erosionSettings_t(){};
};

class erosionScene : public texturedScene
{
public:

	erosionScene(
		erosionSettings_t* erosionSettings = new erosionSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (erosion)",
		camera* erosionCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/erosion.txt")
		: texturedScene(defaultTexture, windowName, erosionCamera, shaderConfigPath)
	{
		this->erosionSettings = erosionSettings;
	}

	~erosionScene(){};

protected:

	erosionSettings_t*		erosionSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		ImGui::SliderFloat("erosion strength X", &erosionSettings->strengthX, 0.0f, 10.0f);
		ImGui::SliderFloat("erosion strength Y", &erosionSettings->strengthY, 0.0f, 10.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(erosionSettings, erosionSettings->bufferHandle, sizeof(*erosionSettings), 1, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(erosionSettings, erosionSettings->bufferHandle, sizeof(*erosionSettings), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}
};

#endif