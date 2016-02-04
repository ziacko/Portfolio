#ifndef CELL_SHADING_H
#define CELL_SHADING_H
#include <TexturedScene.h>

struct cellShadeSettings_t
{
	GLfloat		redModifier;
	GLfloat		greenModifier;
	GLfloat		blueModifier;
	GLfloat		cellDistance;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	cellShadeSettings_t(GLfloat cellDistance = 1.0f, GLfloat redModifier = 1.0f,
		GLfloat greenModifier = 1.0f, GLfloat blueModifier = 1.0f)
	{
		this->redModifier = redModifier;
		this->greenModifier = greenModifier;
		this->blueModifier = blueModifier;
		this->cellDistance = cellDistance;
	}

	~cellShadeSettings_t( void ){};
};

class cellShadingScene : public texturedScene
{
public:

	cellShadingScene(cellShadeSettings_t* cellShade = new cellShadeSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (texture settings)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "./shaders/cellShading.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->cellShadeSettingsBuffer = cellShade;
	}

	~cellShadingScene(void){};

protected:

	cellShadeSettings_t*		cellShadeSettingsBuffer;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "red modifier", TwType::TW_TYPE_FLOAT, &cellShadeSettingsBuffer->redModifier, "min=-1 max=1 step=0.0001");
		TwAddVarRW(tweakBar, "green modifier", TwType::TW_TYPE_FLOAT, &cellShadeSettingsBuffer->greenModifier, "min=-1 max=1 step=0.0001");
		TwAddVarRW(tweakBar, "blue modifier", TwType::TW_TYPE_FLOAT, &cellShadeSettingsBuffer->blueModifier, "min=-1 max=1 step=0.0001");
		TwAddVarRW(tweakBar, "cell distance", TwType::TW_TYPE_FLOAT, &cellShadeSettingsBuffer->cellDistance, "min=-1 max=1 step=0.0001");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<cellShadeSettings_t>(cellShadeSettingsBuffer, cellShadeSettingsBuffer->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<cellShadeSettings_t>(cellShadeSettingsBuffer, cellShadeSettingsBuffer->bufferHandle);
	}
};

#endif