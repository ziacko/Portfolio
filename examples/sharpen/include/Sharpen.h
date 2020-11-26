#ifndef SHARPEN_H
#define SHARPEN_H
#include <TexturedScene.h>

struct sharpenSettings_t
{
	GLfloat			kernel1;
	GLfloat			kernel2;
/*
	GLfloat			kernel3;
	GLfloat			kernel4;
	GLfloat			kernel5;
	GLfloat			kernel6;
	GLfloat			kernel7;
	GLfloat			kernel8;
	GLfloat			kernel9;*/

	sharpenSettings_t(
		GLfloat kernel1 = 0.0f, GLfloat kernel2 = 1.0f/*, GLfloat kernel3 = -1.0f,
		GLfloat kernel4 = -1.0f, GLfloat kernel5 = 9.0f, GLfloat kernel6 = -1.0f,
		GLfloat kernel7 = -1.0f, GLfloat kernel8 = -1.0f, GLfloat kernel9 = -1.0f*/)
	{
		this->kernel1 = kernel1;
		this->kernel2 = kernel2;
/*
		this->kernel3 = kernel3;
		this->kernel4 = kernel4;
		this->kernel5 = kernel5;
		this->kernel6 = kernel6;
		this->kernel7 = kernel7;
		this->kernel8 = kernel8;
		this->kernel9 = kernel9;*/
	}

	~sharpenSettings_t(){ };
};

class sharpenScene : public texturedScene
{
public:

	sharpenScene(
		bufferHandler_t<sharpenSettings_t> sharpenSettings = bufferHandler_t<sharpenSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (sharpen)",
		camera* sharpencamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Sharpen.txt")
		: texturedScene(defaultTexture, windowName, sharpencamera, shaderConfigPath)
	{
		this->sharpen = sharpenSettings;
	}

	~sharpenScene(){};

protected:

	bufferHandler_t<sharpenSettings_t>		sharpen;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::SliderFloat("kernel 1", &sharpen.data.kernel1, -10.0f, 10.0f);
		ImGui::SliderFloat("kernel 2", &sharpen.data.kernel2, -10.0f, 10.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		sharpen.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		sharpen.Update();
	}
};

#endif
