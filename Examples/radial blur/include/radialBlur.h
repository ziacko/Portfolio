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

	GLuint			bufferHandle;
	GLuint			uniformHandle;

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

	radialScene(radialBlur_t* radial = new radialBlur_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (radial blur)",
		camera* radialCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/radialBlurring.txt") :
		texturedScene(defaultTexture, windowName, radialCamera, shaderConfigPath)
	{
		this->radialBlurSettingsBuffer = radial;
	}

	~radialScene(){};

protected:

	static radialBlur_t*		radialBlurSettingsBuffer;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		ImGui::SliderFloat("exposure", &radialBlurSettingsBuffer->exposure, 0.0f, 1.0f);
		ImGui::SliderFloat("decay", &radialBlurSettingsBuffer->decay, 0.0f, 1.0f);
		ImGui::SliderFloat("density", &radialBlurSettingsBuffer->density, 0.0f, 0.01f, "%.10f", 100.0f);
		ImGui::SliderFloat("weight", &radialBlurSettingsBuffer->weight, 0.0f, 10.0f);
		ImGui::SliderInt("samples", &radialBlurSettingsBuffer->samples, 0, 1000);
	}

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "exposure", TwType::TW_TYPE_FLOAT, &radialBlurSettingsBuffer->exposure, "min=0 max=1 step=0.001");
		TwAddVarRW(tweakBar, "decay", TwType::TW_TYPE_FLOAT, &radialBlurSettingsBuffer->decay, "min=0 max=1 step=0.001");
		TwAddVarRW(tweakBar, "density", TwType::TW_TYPE_FLOAT, &radialBlurSettingsBuffer->density, "min=0 max=1 step=0.00001");
		TwAddVarRW(tweakBar, "weight", TwType::TW_TYPE_FLOAT, &radialBlurSettingsBuffer->weight, "min=0 max=1 step=0.001");
		//TwAddVarRW(tweakBar, "attenuation", TwType::TW_TYPE_FLOAT, &radialBlurSettingsBuffer->attenuation, "min=0 max=100 step=0.01");
		TwAddVarRW(tweakBar, "samples", TwType::TW_TYPE_UINT16, &radialBlurSettingsBuffer->samples, "min=0 max=1000");
	}*/

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<radialBlur_t>(radialBlurSettingsBuffer, radialBlurSettingsBuffer->bufferHandle, 1);
	}

	void Update() override
	{
		scene::Update();
		UpdateUniformBuffer<radialBlur_t>(radialBlurSettingsBuffer, radialBlurSettingsBuffer->bufferHandle);
	}
};

radialBlur_t* radialScene::radialBlurSettingsBuffer = nullptr;


#endif