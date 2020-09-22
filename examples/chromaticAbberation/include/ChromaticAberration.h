#ifndef CHROMATICABERRATION_H
#define CHROMATICABERRATION_H
#include <TexturedScene.h>

struct chromaticSettings_t
{
	float			redOffset;
	float			blueOffset;
	float			greenOffset;

	chromaticSettings_t(GLfloat redOffset = 0.0f, GLfloat greenOffset = 0.0f, GLfloat blueOffset = 0.0f)
	{
		this->redOffset = redOffset;
		this->greenOffset = greenOffset;
		this->blueOffset = blueOffset;
	}

	~chromaticSettings_t(){};
};

class chromaticScene : public texturedScene
{

public:

	chromaticScene(
		chromaticSettings_t chromaticSettings = chromaticSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (chromatic aberration)",
		camera* chromaticCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Chromatic.txt") : texturedScene(defaultTexture, windowName, chromaticCamera, shaderConfigPath)
	{
		this->tweakBarName = windowName;
		this->chromaticSettings = chromaticSettings;
	}

	~chromaticScene( void ){}

protected:

	bufferHandler_t<chromaticSettings_t>	chromaticSettings;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::SliderFloat("red offset", &chromaticSettings.data.redOffset, -1.0f, 1.0f, "%0.10f");
		ImGui::SliderFloat("green offset", &chromaticSettings.data.greenOffset, -1.0f, 1.0f, "%0.10f");
		ImGui::SliderFloat("blue offset", &chromaticSettings.data.blueOffset, -1.0f, 1.0f, "%0.10f");
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		chromaticSettings.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		chromaticSettings.Update(gl_uniform_buffer, gl_dynamic_draw);
	}
};
#endif