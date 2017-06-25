#ifndef GAUSSIAN_BLURRING_H
#define GAUSSIAN_BLURRING_H
#include <TexturedScene.h>

struct gaussianSettings_t
{
	int			numOffsets;
	float		weight;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	gaussianSettings_t(GLuint numOffsets = 5, GLfloat weight = 0.1f)
	{
		this->numOffsets = numOffsets;
		this->weight = weight;
	}

	~gaussianSettings_t(){};
};

class gaussianScene : public texturedScene
{
public:

	gaussianScene(gaussianSettings_t* gaussian = new gaussianSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (gaussian blurring)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Gaussian.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->gaussianSettingsBuffer = gaussian;
	}

	~gaussianScene(void){};

protected:

	static	gaussianSettings_t*		gaussianSettingsBuffer;

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);
		//need to come back and set up a 2 render pass system for proper gaussian blurring
		//ImGui::SliderInt("num offsets", &gaussianSettingsBuffer->numOffsets, 0, 100);
		ImGui::SliderFloat("weight", &gaussianSettingsBuffer->weight, 0.0f, 1.0f);
	}

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "blur weight", TwType::TW_TYPE_FLOAT, &gaussianSettingsBuffer->weight, "min=0 max=1 step=0.001");
	}*/

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(gaussianSettingsBuffer, gaussianSettingsBuffer->bufferHandle, sizeof(*gaussianSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(gaussianSettingsBuffer, gaussianSettingsBuffer->bufferHandle, sizeof(*gaussianSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);
	}
};

gaussianSettings_t* gaussianScene::gaussianSettingsBuffer = nullptr;

#endif