#ifndef DILATION_H
#define DILATION_H
#include <TexturedScene.h>

struct dilationSettings_t
{
	float			strengthX;
	float			strengthY;

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
		bufferHandler_t<dilationSettings_t> dilationSettings = bufferHandler_t<dilationSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (dilation)",
		camera* dilationCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Dilation.txt")
		: texturedScene(defaultTexture, windowName, dilationCamera, shaderConfigPath)
	{
		this->dilation = dilationSettings;
	}

	~dilationScene(){};

protected:

	bufferHandler_t<dilationSettings_t>		dilation;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::SliderFloat("dilation strength X", &dilation.data.strengthX, 0.0f, 10.0f);
		ImGui::SliderFloat("dilation strength Y", &dilation.data.strengthY, 0.0f, 10.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		dilation.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		dilation.Update(gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif
