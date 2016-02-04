#ifndef PIXELIZE_H
#define PIXELIZE_H
#include <TexturedScene.h>

struct pixelizeSettings_t
{
	GLfloat			pixelWidth;
	GLfloat			pixelHeight;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	pixelizeSettings_t(GLfloat pixelWidth = 10.0f, GLfloat pixelHeight = 10.0f)
	{
		this->pixelWidth = pixelWidth;
		this->pixelHeight = pixelHeight;
	}

	~pixelizeSettings_t(){ };
};

class pixelizeScene : public texturedScene
{
public:

	pixelizeScene(
		pixelizeSettings_t* pixelizeSettings = new pixelizeSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (pixelize)",
		camera* pixelizeCamera = new camera(),
		const char* shaderConfigPath = "./shaders/Pixelize.txt")
		: texturedScene(defaultTexture, windowName, pixelizeCamera, shaderConfigPath)
	{
		this->pixelizeSettings = pixelizeSettings;
	}

	~pixelizeScene(void){}

protected:

	pixelizeSettings_t*		pixelizeSettings;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "pixel width", TwType::TW_TYPE_FLOAT, &pixelizeSettings->pixelWidth, "min=0 step=0.01");
		TwAddVarRW(tweakBar, "pixel height", TwType::TW_TYPE_FLOAT, &pixelizeSettings->pixelHeight, "min=0 step=0.01");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<pixelizeSettings_t>(pixelizeSettings, pixelizeSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<pixelizeSettings_t>(pixelizeSettings, pixelizeSettings->bufferHandle);
	}

};
#endif