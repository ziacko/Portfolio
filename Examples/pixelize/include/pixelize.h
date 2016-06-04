#ifndef PIXELIZE_H
#define PIXELIZE_H
#include <TexturedScene.h>

struct pixelizeSettings_t
{
	GLfloat			pixelWidth;
	GLfloat			pixelHeight;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	pixelizeSettings_t(GLfloat pixelWidth = 8.0f, GLfloat pixelHeight = 8.0f)
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
		const char* shaderConfigPath = "../../resources/shaders/Pixelize.txt")
		: texturedScene(defaultTexture, windowName, pixelizeCamera, shaderConfigPath)
	{
		this->pixelizeSettings = pixelizeSettings;
	}

	~pixelizeScene(void){}

protected:

	pixelizeSettings_t*		pixelizeSettings;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);

		ImGui::SliderFloat("pixel width", &pixelizeSettings->pixelWidth, 0.0f, 100.0f, "%.0f");
		ImGui::SliderFloat("pixel height", &pixelizeSettings->pixelHeight, 0.0f, 100.0f, "%.0f");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(pixelizeSettings, pixelizeSettings->bufferHandle, sizeof(*pixelizeSettings), 1, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(pixelizeSettings, pixelizeSettings->bufferHandle, sizeof(*pixelizeSettings), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

};
#endif