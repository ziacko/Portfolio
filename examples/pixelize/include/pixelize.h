#ifndef PIXELIZE_H
#define PIXELIZE_H
#include <TexturedScene.h>

struct pixelizeSettings_t
{
	GLfloat			pixelWidth;
	GLfloat			pixelHeight;

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
		bufferHandler_t<pixelizeSettings_t> pixelizeSettings = bufferHandler_t<pixelizeSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (pixelize)",
		camera* pixelizeCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Pixelize.txt")
		: texturedScene(defaultTexture, windowName, pixelizeCamera, shaderConfigPath)
	{
		this->pixel = pixelizeSettings;
	}

	~pixelizeScene(void){}

protected:

	bufferHandler_t<pixelizeSettings_t>		pixel;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::SliderFloat("pixel width", &pixel.data.pixelWidth, 0.0f, 100.0f, "%.0f");
		ImGui::SliderFloat("pixel height", &pixel.data.pixelHeight, 0.0f, 100.0f, "%.0f");
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		pixel.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		pixel.Update();
	}

};
#endif