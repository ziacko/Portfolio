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

	GLuint			bufferHandle;
	GLuint			uniformHandle;

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
		sepiaSettings_t* sepiaSettings = new sepiaSettings_t(),
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

	sepiaSettings_t*		sepiaSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);

		ImGui::SliderFloat("factor", &sepiaSettings->factor, 0.0f, 1.0f);

		ImGui::SliderFloat("red modifier 1", &sepiaSettings->redModifier1, 0.0f, 1.0f);
		ImGui::SliderFloat("red modifier 2", &sepiaSettings->redModifier2, 0.0f, 1.0f);
		ImGui::SliderFloat("red modifier 3", &sepiaSettings->redModifier3, 0.0f, 1.0f);

		ImGui::SliderFloat("green modifier 1", &sepiaSettings->greenModifier1, 0.0f, 1.0f);
		ImGui::SliderFloat("green modifier 2", &sepiaSettings->greenModifier2, 0.0f, 1.0f);
		ImGui::SliderFloat("green modifier 3", &sepiaSettings->greenModifier3, 0.0f, 1.0f);

		ImGui::SliderFloat("blue modifier 1", &sepiaSettings->blueModifier1, 0.0f, 1.0f);
		ImGui::SliderFloat("blue modifier 2", &sepiaSettings->blueModifier2, 0.0f, 1.0f);
		ImGui::SliderFloat("blue modifier 3", &sepiaSettings->blueModifier3, 0.0f, 1.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(sepiaSettings, sepiaSettings->bufferHandle, sizeof(*sepiaSettings), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(sepiaSettings, sepiaSettings->bufferHandle, sizeof(*sepiaSettings), gl_uniform_buffer, gl_dynamic_draw);
	}
};
#endif