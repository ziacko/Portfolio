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

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "kernel 1", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel1, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 2", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel2, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 3", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel3, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 4", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel4, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 5", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel5, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 6", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel6, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 7", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel7, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 8", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel8, "min=-100 max=100 step=0.01");
		TwAddVarRW(tweakBar, "kernel 9", TwType::TW_TYPE_FLOAT, &sharpenSettings->kernel9, "min=-100 max=100 step=0.01");
	}*/

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