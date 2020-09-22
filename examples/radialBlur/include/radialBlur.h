#ifndef RADIAL_BLUR_H
#define RADIAL_BLUR_H
#include <TexturedScene.h>

struct radialBlur_t
{
	GLfloat		exposure;
	GLfloat		decay;
	GLfloat		density;
	GLfloat		weight;
	int			samples;

	radialBlur_t(GLfloat exposure = 0.01f, GLfloat decay = 0.975f, GLfloat density = 0.00005f,
		GLfloat weight = 1.0f, GLuint samples = 100)
	{
		this->exposure = exposure;
		this->decay = decay;
		this->density = density;
		this->weight = weight;
		this->samples = samples;
	}

	~radialBlur_t( void ){};
};

class radialScene : public texturedScene
{
public:

	radialScene(bufferHandler_t<radialBlur_t> radial = bufferHandler_t<radialBlur_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (radial blur)",
		camera* radialCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/RadialBlurring.txt") :
		texturedScene(defaultTexture, windowName, radialCamera, shaderConfigPath)
	{
		this->radialBlur = radial;
	}

	~radialScene(){};

protected:

	bufferHandler_t<radialBlur_t>		radialBlur;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::SliderFloat("exposure", &radialBlur.data.exposure, 0.0f, 1.0f);
		ImGui::SliderFloat("decay", &radialBlur.data.decay, 0.0f, 1.0f);
		ImGui::SliderFloat("density", &radialBlur.data.density, 0.0f, 0.01f, "%.10f", 100.0f);
		ImGui::SliderFloat("weight", &radialBlur.data.weight, 0.0f, 10.0f);
		ImGui::SliderInt("samples", &radialBlur.data.samples, 0, 1000);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		radialBlur.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		radialBlur.Update();
	}
};
#endif