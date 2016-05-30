#ifndef SHARPEN_H
#define SHARPEN_H
#include <TexturedScene.h>

struct sharpenSettings_t
{
	GLfloat			kernel1;
	GLfloat			kernel2;
	GLfloat			kernel3;
	GLfloat			kernel4;
	GLfloat			kernel5;
	GLfloat			kernel6;
	GLfloat			kernel7;
	GLfloat			kernel8;
	GLfloat			kernel9;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	sharpenSettings_t(
		GLfloat kernel1 = -1.0f, GLfloat kernel2 = -1.0f, GLfloat kernel3 = -1.0f,
		GLfloat kernel4 = -1.0f, GLfloat kernel5 = 9.0f, GLfloat kernel6 = -1.0f,
		GLfloat kernel7 = -1.0f, GLfloat kernel8 = -1.0f, GLfloat kernel9 = -1.0f)
	{
		this->kernel1 = kernel1;
		this->kernel2 = kernel2;
		this->kernel3 = kernel3;
		this->kernel4 = kernel4;
		this->kernel5 = kernel5;
		this->kernel6 = kernel6;
		this->kernel7 = kernel7;
		this->kernel8 = kernel8;
		this->kernel9 = kernel9;
	}

	~sharpenSettings_t(){ };
};


class sharpenScene : public texturedScene
{
public:

	sharpenScene(
		sharpenSettings_t* sharpenSettings = new sharpenSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (sharpen)",
		camera* sharpencamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/sharpen.txt")
		: texturedScene(defaultTexture, windowName, sharpencamera, shaderConfigPath)
	{
		this->sharpenSettings = sharpenSettings;
	}

	~sharpenScene(){};

protected:

	sharpenSettings_t*			sharpenSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);

		ImGui::SliderFloat("kernel 1", &sharpenSettings->kernel1, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 2", &sharpenSettings->kernel2, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 3", &sharpenSettings->kernel3, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 4", &sharpenSettings->kernel4, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 5", &sharpenSettings->kernel5, -100.0f, 100.0f);
		ImGui::SliderFloat("kernel 6", &sharpenSettings->kernel6, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 7", &sharpenSettings->kernel7, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 8", &sharpenSettings->kernel8, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 9", &sharpenSettings->kernel9, -1.0f, 1.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<sharpenSettings_t>(sharpenSettings, sharpenSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<sharpenSettings_t>(sharpenSettings, sharpenSettings->bufferHandle);
	}
};

#endif