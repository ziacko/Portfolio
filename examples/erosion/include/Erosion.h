#ifndef EROSION_H
#define EROSION_H
#include <TexturedScene.h>

struct erosionSettings_t
{
	float			strengthX;
	float			strengthY;

	erosionSettings_t(GLfloat strengthX = 1.0f, GLfloat strengthY = 1.0f)
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
		erosionSettings_t erosionSettings = erosionSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (erosion)",
		camera* erosionCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Erosion.txt")
		: texturedScene(defaultTexture, windowName, erosionCamera, shaderConfigPath)
	{
		this->erosion = erosionSettings;
	}

	~erosionScene(){};

protected:

	bufferHandler_t<erosionSettings_t>	erosion;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(windows[0], io);
		ImGui::SliderFloat("erosion strength X", &erosion.data.strengthX, 0.0f, 10.0f);
		ImGui::SliderFloat("erosion strength Y", &erosion.data.strengthY, 0.0f, 10.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		erosion.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		erosion.Update(gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif