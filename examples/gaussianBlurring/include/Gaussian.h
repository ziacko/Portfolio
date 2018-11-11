#ifndef GAUSSIAN_BLURRING_H
#define GAUSSIAN_BLURRING_H
#include <TexturedScene.h>

struct gaussianSettings_t
{
	//damn 5 points is so fkn awkward
	GLint		offset1;
	GLint		offset2;
	GLint		offset3;
	GLint		offset4;
	GLint		offset5;

	float		weight1;
	float		weight2;
	float		weight3;
	float		weight4;
	float		weight5;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	gaussianSettings_t(GLint offset1 = 0, GLint offset2 = 1, GLint offset3 = 2, GLint offset4 = 3, GLint offset5 = 4,
		GLfloat weight1 = 0.2270270270f, GLfloat weight2 = 0.1945945946, GLfloat weight3 = 0.1216216216, GLfloat weight4 = 0.0540540541, GLfloat weight5 = 0.0162162162)
	{
		this->offset1 = offset1;
		this->offset2 = offset2;
		this->offset3 = offset3;
		this->offset4 = offset4;
		this->offset5 = offset5;

		this->weight1 = weight1;
		this->weight2 = weight2;
		this->weight3 = weight3;
		this->weight4 = weight4;
		this->weight5 = weight5;

		bufferHandle = 0;
		uniformHandle = 0;
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

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat("weight1", &gaussianSettingsBuffer->weight1, 0.0f, 0.25f, "%.10f");	 
		ImGui::SameLine();		
		ImGui::SliderFloat("weight2", &gaussianSettingsBuffer->weight2, 0.0f, 0.25f, "%.10f");
		
		ImGui::SliderFloat("weight3", &gaussianSettingsBuffer->weight3, 0.0f, 0.25f, "%.10f");
		ImGui::SameLine();
		ImGui::SliderFloat("weight4", &gaussianSettingsBuffer->weight4, 0.0f, 0.25f, "%.10f");
		ImGui::SliderFloat("weight5", &gaussianSettingsBuffer->weight5, 0.0f, 0.25f, "%.10f");
		ImGui::PopItemWidth();

		ImGui::SliderInt("offset1", &gaussianSettingsBuffer->offset1, 0, 100);
		ImGui::SliderInt("offset2", &gaussianSettingsBuffer->offset2, 0, 100);
		ImGui::SliderInt("offset3", &gaussianSettingsBuffer->offset3, 0, 100);
		ImGui::SliderInt("offset4", &gaussianSettingsBuffer->offset4, 0, 100);
		ImGui::SliderInt("offset5", &gaussianSettingsBuffer->offset5, 0, 100);
	}

	void InitializeBuffers() override
	{
		scene::InitializeUniforms();
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