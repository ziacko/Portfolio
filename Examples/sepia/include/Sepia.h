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
		const char* shaderConfigPath = "./shaders/Sepia.txt")
		: texturedScene(defaultTexture, windowName, sepiaCamera, shaderConfigPath)
	{
		this->sepiaSettings = sepiaSettings;
	}

	~sepiaScene(){};

protected:

	sepiaSettings_t*		sepiaSettings;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "factor", TwType::TW_TYPE_FLOAT, &sepiaSettings->factor, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "red modifier 1", TwType::TW_TYPE_FLOAT, &sepiaSettings->redModifier1, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "red modifier 2 ", TwType::TW_TYPE_FLOAT, &sepiaSettings->redModifier2, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "red modifier 3", TwType::TW_TYPE_FLOAT, &sepiaSettings->redModifier3, "min=0 max=1 step=0.01");

		TwAddVarRW(tweakBar, "green modifier 1", TwType::TW_TYPE_FLOAT, &sepiaSettings->greenModifier1, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "green modifier 2", TwType::TW_TYPE_FLOAT, &sepiaSettings->greenModifier2, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "green modifier 3", TwType::TW_TYPE_FLOAT, &sepiaSettings->greenModifier3, "min=0 max=1 step=0.01");

		TwAddVarRW(tweakBar, "blue modifier 1", TwType::TW_TYPE_FLOAT, &sepiaSettings->blueModifier1, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "blue modifier 2", TwType::TW_TYPE_FLOAT, &sepiaSettings->blueModifier2, "min=0 max=1 step=0.01");
		TwAddVarRW(tweakBar, "blue modifier 3", TwType::TW_TYPE_FLOAT, &sepiaSettings->blueModifier3, "min=0 max=1 step=0.01");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<sepiaSettings_t>(sepiaSettings, sepiaSettings->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<sepiaSettings_t>(sepiaSettings, sepiaSettings->bufferHandle);
	}
};


#endif