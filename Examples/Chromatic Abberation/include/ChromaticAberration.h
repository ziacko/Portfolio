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
		this->chromaticSettings = chromaticSettings;
	}

	~chromaticScene( void ){}

protected:

	static chromaticSettings_t*		chromaticSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);

		ImGui::SliderFloat("red offset", &chromaticSettings->redOffset, -1.0f, 1.0f, "%0.10f");
		ImGui::SliderFloat("green offset", &chromaticSettings->greenOffset, -1.0f, 1.0f, "%0.10f");
		ImGui::SliderFloat("blue offset", &chromaticSettings->blueOffset, -1.0f, 1.0f, "%0.10f");
	}

	void SetupChromaticBuffer()
	{
		glGenBuffers(1, &chromaticSettings->bufferHandle);

		glBindBuffer(GL_UNIFORM_BUFFER, chromaticSettings->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(*chromaticSettings), chromaticSettings, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->chromaticSettings->bufferHandle);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupChromaticBuffer();
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(chromaticSettings, chromaticSettings->bufferHandle, sizeof(*chromaticSettings), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}
};

chromaticSettings_t* chromaticScene::chromaticSettings = nullptr;

#endif