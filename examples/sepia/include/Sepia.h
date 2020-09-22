#ifndef SEPIA_H
#define SEPIA_H
#include <TexturedScene.h>

struct sepiaSettings_t
{
	GLfloat			factor;
	
	GLfloat			redModifier1;
	GLfloat			redModifier2;
	GLfloat			redModifier3;

	GLfloat			greenModifier1;
	GLfloat			greenModifier2;
	GLfloat			greenModifier3;

	GLfloat			blueModifier1;
	GLfloat			blueModifier2;
	GLfloat			blueModifier3;

	sepiaSettings_t(GLfloat factor = 0.75f,
		GLfloat redModifier1 = 0.393f, GLfloat redModifier2 = 0.349f, GLfloat redModifier3 = 0.272f,
		GLfloat greenModifier1 = 0.769f, GLfloat greenModifier2 = 0.686f, GLfloat greenModifier3 = 0.534f,
		GLfloat blueModifier1 = 0.189f, GLfloat blueModifier2 = 0.168f, GLfloat blueModifier3 = 0.131f)
	{
		this->factor = factor;
		this->redModifier1 = redModifier1;
		this->redModifier2 = redModifier2;
		this->redModifier3 = redModifier3;

		this->greenModifier1 = greenModifier1;
		this->greenModifier2 = greenModifier2;
		this->greenModifier3 = greenModifier3;

		this->blueModifier1 = blueModifier1;
		this->blueModifier2 = blueModifier2;
		this->blueModifier3 = blueModifier3;
	}

	~sepiaSettings_t(){};
};

class sepiaScene : public texturedScene
{
public:

	sepiaScene(
		bufferHandler_t<sepiaSettings_t> sepiaSettings = bufferHandler_t<sepiaSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (sepia)",
		camera* sepiaCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Sepia.txt")
		: texturedScene(defaultTexture, windowName, sepiaCamera, shaderConfigPath)
	{
		this->sepiaSettings = sepiaSettings;
	}

	~sepiaScene(){};

protected:

	bufferHandler_t<sepiaSettings_t>  sepiaSettings;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::SliderFloat("factor", &sepiaSettings.data.factor, 0.0f, 1.0f);

		ImGui::SliderFloat("red modifier 1", &sepiaSettings.data.redModifier1, 0.0f, 1.0f);
		ImGui::SliderFloat("red modifier 2", &sepiaSettings.data.redModifier2, 0.0f, 1.0f);
		ImGui::SliderFloat("red modifier 3", &sepiaSettings.data.redModifier3, 0.0f, 1.0f);

		ImGui::SliderFloat("green modifier 1", &sepiaSettings.data.greenModifier1, 0.0f, 1.0f);
		ImGui::SliderFloat("green modifier 2", &sepiaSettings.data.greenModifier2, 0.0f, 1.0f);
		ImGui::SliderFloat("green modifier 3", &sepiaSettings.data.greenModifier3, 0.0f, 1.0f);

		ImGui::SliderFloat("blue modifier 1", &sepiaSettings.data.blueModifier1, 0.0f, 1.0f);
		ImGui::SliderFloat("blue modifier 2", &sepiaSettings.data.blueModifier2, 0.0f, 1.0f);
		ImGui::SliderFloat("blue modifier 3", &sepiaSettings.data.blueModifier3, 0.0f, 1.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		sepiaSettings.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		sepiaSettings.Update();
	}
};
#endif