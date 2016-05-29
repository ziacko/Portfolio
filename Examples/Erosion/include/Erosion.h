#ifndef EROSION_H
#define EROSION_H
#include <TexturedScene.h>

struct erosionSettings_t
{
	GLfloat			strengthX;
	GLfloat			strengthY;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	erosionSettings_t(
		GLfloat strengthX = 1.0f, GLfloat strengthY= 1.0f)
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
		erosionSettings_t* erosionSettings = new erosionSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (erosion)",
		camera* erosionCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/erosion.txt")
		: texturedScene(defaultTexture, windowName, erosionCamera, shaderConfigPath)
	{
		this->erosionSettings = erosionSettings;
	}

	~erosionScene(){};

protected:

	erosionSettings_t*		erosionSettings;

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "erosion strength X", TwType::TW_TYPE_FLOAT, &erosionSettings->strengthX, "min=0 max=10 step=0.01");
		TwAddVarRW(tweakBar, "erosion strength Y", TwType::TW_TYPE_FLOAT, &erosionSettings->strengthY, "min=0 max=10 step=0.01");
	}*/

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<erosionSettings_t>(erosionSettings, erosionSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<erosionSettings_t>(erosionSettings, erosionSettings->bufferHandle);
	}
};

#endif