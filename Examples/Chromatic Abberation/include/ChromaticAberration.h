#ifndef CHROMATICABERRATION_H
#define CHROMATICABERRATION_H
#include <TexturedScene.h>

struct chromaticSettings_t
{
	GLfloat			redOffset;
	GLfloat			blueOffset;
	GLfloat			greenOffset;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	chromaticSettings_t(GLfloat redOffset = 0.0f, GLfloat greenOffset = 0.0f, GLfloat blueOffset = 0.0f)
	{
		this->redOffset = redOffset;
		this->greenOffset = greenOffset;
		this->blueOffset = blueOffset;
	}

	~chromaticSettings_t(){};
};

class chromaticScene : public texturedScene
{

public:

	chromaticScene(
		chromaticSettings_t* chromaticSettings = new chromaticSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (chromatic aberration)",
		camera* chromaticCamera = new camera(),
		const char* shaderConfigPath = "./shaders/Chromatic.txt") : texturedScene(defaultTexture, windowName, chromaticCamera, shaderConfigPath)
	{
		this->tweakBarName = windowName;
		this->chromaticSettingsBuffer = chromaticSettings;
	}

	~chromaticScene( void ){}

protected:

	static chromaticSettings_t*		chromaticSettingsBuffer;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "red offset", TwType::TW_TYPE_FLOAT, &chromaticSettingsBuffer->redOffset, "min=-1 max=1 step=0.0001");
		TwAddVarRW(tweakBar, "green offset", TwType::TW_TYPE_FLOAT, &chromaticSettingsBuffer->greenOffset, "min=-1 max=1 step=0.0001");
		TwAddVarRW(tweakBar, "blue offset", TwType::TW_TYPE_FLOAT, &chromaticSettingsBuffer->blueOffset, "min=-1 max=1 step=0.0001");
	}

	void SetupChromaticBuffer()
	{
		glGenBuffers(1, &chromaticSettingsBuffer->bufferHandle);

		glBindBuffer(GL_UNIFORM_BUFFER, chromaticSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(chromaticSettings_t), chromaticSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->chromaticSettingsBuffer->bufferHandle);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupChromaticBuffer();
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<chromaticSettings_t>(chromaticSettingsBuffer, chromaticSettingsBuffer->bufferHandle);
	}
};

chromaticSettings_t* chromaticScene::chromaticSettingsBuffer = nullptr;


#endif