#ifndef DILATION_H
#define DILATION_H
#include <TexturedScene.h>

struct dilationSettings_t
{
	float			strengthX;
	float			strengthY;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	dilationSettings_t(
		GLfloat strengthX = 1.0f, GLfloat strengthY = 1.0f)
	{
		this->strengthX = strengthX;
		this->strengthY = strengthY;
	}

	~dilationSettings_t(){};
};

class dilationScene : public texturedScene
{
public:

	dilationScene(
		dilationSettings_t* dilationSettings = new dilationSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (dilation)",
		camera* dilationCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Dilation.txt")
		: texturedScene(defaultTexture, windowName, dilationCamera, shaderConfigPath)
	{
		this->dilationSettings = dilationSettings;
	}

	~dilationScene(){};

protected:

	dilationSettings_t*			dilationSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		ImGui::SliderFloat("dilation strength X", &dilationSettings->strengthX, 0.0f, 10.0f);
		ImGui::SliderFloat("dilation strength Y", &dilationSettings->strengthY, 0.0f, 10.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(dilationSettings, dilationSettings->bufferHandle, sizeof(*dilationSettings), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(dilationSettings, dilationSettings->bufferHandle, sizeof(*dilationSettings), gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif
