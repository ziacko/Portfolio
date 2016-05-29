#ifndef DILATION_H
#define DILATION_H
#include <TexturedScene.h>

struct dilationSettings_t
{
	GLfloat			strengthX;
	GLfloat			strengthY;

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
		const char* shaderConfigPath = "../../resources/shaders/dilation.txt")
		: texturedScene(defaultTexture, windowName, dilationCamera, shaderConfigPath)
	{
		this->dilationSettings = dilationSettings;
	}

	~dilationScene(){};

protected:

	dilationSettings_t*			dilationSettings;

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "dilation strength X", TwType::TW_TYPE_FLOAT, &dilationSettings->strengthX, "min=0 max=10 step=0.01");
		TwAddVarRW(tweakBar, "dilation strength Y", TwType::TW_TYPE_FLOAT, &dilationSettings->strengthY, "min=0 max=10 step=0.01");
	}*/

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<dilationSettings_t>(dilationSettings, dilationSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<dilationSettings_t>(dilationSettings, dilationSettings->bufferHandle);
	}
};

#endif