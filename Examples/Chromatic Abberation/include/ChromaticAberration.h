#ifndef CHROMATICABERRATION_H
#define CHROMATICABERRATION_H
#include <TexturedScene.h>

struct chromaticSettings_t
{
	float			redOffset;
	float			blueOffset;
	float			greenOffset;

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
		const char* shaderConfigPath = "../../resources/shaders/Chromatic.txt") : texturedScene(defaultTexture, windowName, chromaticCamera, shaderConfigPath)
	{
		this->tweakBarName = windowName;
		this->chromaticSettingsBuffer = chromaticSettings;
	}

	~chromaticScene( void ){}

protected:

	static chromaticSettings_t*		chromaticSettingsBuffer;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);

		ImGui::SliderFloat("red offset", &chromaticSettingsBuffer->redOffset, -1.0f, 1.0f, "%0.10f");
		ImGui::SliderFloat("green offset", &chromaticSettingsBuffer->greenOffset, -1.0f, 1.0f, "%0.10f");
		ImGui::SliderFloat("blue offset", &chromaticSettingsBuffer->blueOffset, -1.0f, 1.0f, "%0.10f");
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