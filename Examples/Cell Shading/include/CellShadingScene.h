#ifndef CELL_SHADING_H
#define CELL_SHADING_H
#include <TexturedScene.h>

struct cellShadeSettings_t
{
	float		redModifier;
	float		greenModifier;
	float		blueModifier;
	float		cellDistance;

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
		const char* shaderConfigPath = "../../resources/shaders/cellShading.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->cellShadeSettingsBuffer = cellShade;
	}

	~cellShadingScene(void){};

protected:

	cellShadeSettings_t*		cellShadeSettingsBuffer;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		ImGui::SliderFloat("red modifier", &cellShadeSettingsBuffer->redModifier, 0.0f, 1.0f);
		ImGui::SliderFloat("green modifier", &cellShadeSettingsBuffer->greenModifier, 0.0f, 1.0f);
		ImGui::SliderFloat("blue modifier", &cellShadeSettingsBuffer->blueModifier, 0.0f, 1.0f);
		ImGui::SliderFloat("cell distance", &cellShadeSettingsBuffer->cellDistance, 0.0f, 1.0f);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(cellShadeSettingsBuffer, cellShadeSettingsBuffer->bufferHandle, sizeof(*cellShadeSettingsBuffer), 1, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(cellShadeSettingsBuffer, cellShadeSettingsBuffer->bufferHandle, sizeof(*cellShadeSettingsBuffer), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}
};

#endif