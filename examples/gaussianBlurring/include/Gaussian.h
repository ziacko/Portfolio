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
	}

	~gaussianSettings_t(){};
};

class gaussianScene : public texturedScene
{
public:

	gaussianScene(bufferHandler_t<gaussianSettings_t> gaussian = bufferHandler_t<gaussianSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (gaussian blurring)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Gaussian.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->gaussian = gaussian;
	}

	~gaussianScene(void){};

protected:

	bufferHandler_t<gaussianSettings_t>		gaussian;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		
		ImGui::PushItemWidth(100.0f);
		ImGui::SliderFloat("weight1", &gaussian.data.weight1, 0.0f, 0.25f, "%.10f");	 
		ImGui::SameLine();		
		ImGui::SliderFloat("weight2", &gaussian.data.weight2, 0.0f, 0.25f, "%.10f");
		
		ImGui::SliderFloat("weight3", &gaussian.data.weight3, 0.0f, 0.25f, "%.10f");
		ImGui::SameLine();
		ImGui::SliderFloat("weight4", &gaussian.data.weight4, 0.0f, 0.25f, "%.10f");
		ImGui::SliderFloat("weight5", &gaussian.data.weight5, 0.0f, 0.25f, "%.10f");
		ImGui::PopItemWidth();

		ImGui::SliderInt("offset1", &gaussian.data.offset1, 0, 100);
		ImGui::SliderInt("offset2", &gaussian.data.offset2, 0, 100);
		ImGui::SliderInt("offset3", &gaussian.data.offset3, 0, 100);
		ImGui::SliderInt("offset4", &gaussian.data.offset4, 0, 100);
		ImGui::SliderInt("offset5", &gaussian.data.offset5, 0, 100);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		gaussian.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		gaussian.Update();
	}
};

#endif