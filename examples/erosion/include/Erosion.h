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
		const char* shaderConfigPath = "../../resources/shaders/Erosion.txt")
		: texturedScene(defaultTexture, windowName, erosionCamera, shaderConfigPath)
	{
		this->erosionSettings = erosionSettings;
	}

	~erosionScene(){};

protected:

	erosionSettings_t*		erosionSettings;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(windows[0], io);
		ImGui::SliderFloat("erosion strength X", &erosionSettings->strengthX, 0.0f, 10.0f);
		ImGui::SliderFloat("erosion strength Y", &erosionSettings->strengthY, 0.0f, 10.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeUniforms();
		SetupBuffer(erosionSettings, erosionSettings->bufferHandle, sizeof(*erosionSettings), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(erosionSettings, erosionSettings->bufferHandle, sizeof(*erosionSettings), gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif