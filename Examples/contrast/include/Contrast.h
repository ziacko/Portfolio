#ifndef CONTRAST_H
#define CONTRAST_H

#include <TexturedScene.h>

struct contrastSettings_t
{
	GLfloat			contrast;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	contrastSettings_t(GLfloat contrast = 1.0f)
	{
		this->contrast = contrast;
	}

	~contrastSettings_t(){};
};

class contrastScene : public texturedScene
{
public:

	contrastScene(
		contrastSettings_t* contrastSettings = new contrastSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (contrast)",
		camera* contrastCamera = new camera(),
		const char* shaderConfigPath = "./shaders/contrast.txt")
		: texturedScene(defaultTexture, windowName, contrastCamera, shaderConfigPath)
	{
		this->contrastSettings = contrastSettings;
	}

	~contrastScene(){};

protected:

	contrastSettings_t*			contrastSettings;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "contrast", TwType::TW_TYPE_FLOAT, &contrastSettings->contrast, "min=0 max=10 step=0.01");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<contrastSettings_t>(contrastSettings, contrastSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<contrastSettings_t>(contrastSettings, contrastSettings->bufferHandle);
	}

};




#endif