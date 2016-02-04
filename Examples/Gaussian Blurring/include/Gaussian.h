#ifndef GAUSSIAN_BLURRING_H
#define GAUSSIAN_BLURRING_H
#include <TexturedScene.h>

struct gaussianSettings_t
{
	GLuint		numOffsets;
	GLfloat		weight;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	gaussianSettings_t(GLuint numOffsets = 5, GLfloat weight = 0.0f)
	{
		this->numOffsets = numOffsets;
		this->weight = weight;
	}

	~gaussianSettings_t(){};
};

class gaussianScene : public texturedScene
{
public:

	gaussianScene(gaussianSettings_t* gaussian = new gaussianSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (gaussian blurring)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "./shaders/gaussian.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->gaussianSettingsBuffer = gaussian;
	}

	~gaussianScene(void){};

protected:

	static	gaussianSettings_t*		gaussianSettingsBuffer;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "blur weight", TwType::TW_TYPE_FLOAT, &gaussianSettingsBuffer->weight, "min=0 max=1 step=0.001");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<gaussianSettings_t>(gaussianSettingsBuffer, gaussianSettingsBuffer->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<gaussianSettings_t>(gaussianSettingsBuffer, gaussianSettingsBuffer->bufferHandle);
	}
};

gaussianSettings_t* gaussianScene::gaussianSettingsBuffer = nullptr;

#endif