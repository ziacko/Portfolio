#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H
#include <TexturedScene.h>

typedef enum {SOBEL = 0, LAPLACIAN, PREWITT, FREI_CHEN} edgeFilters_t; //ooh right this was never finished. crap!

struct sobelSettings_t
{
	float		redModifier;
	float		greenModifier;
	float		blueModifier;
	float		cellDistance;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	sobelSettings_t(GLfloat cellDistance = 0.25f, GLfloat redModifier = 1.0f,
		GLfloat greenModifier = 1.0f, GLfloat blueModifier = 1.0f)
	{
		this->redModifier = redModifier;
		this->greenModifier = greenModifier;
		this->blueModifier = blueModifier;
		this->cellDistance = cellDistance;
	}

	~sobelSettings_t(void) {};
};

struct laplacianSettings_t
{
	float kernel1;
	float kernel2;
	float kernel3;
	float kernel4;
	float kernel5;
	float kernel6;
	float kernel7;
	float kernel8;
	float kernel9;

	float filterLevel = 0.25f;


	GLuint bufferHandle;
	GLuint uniformHandle;

	laplacianSettings_t(float kernel1 = 0.0f, float kernel2 = 1.0f, float kernel3 = 0.0f,
		float kernel4 = 1.0f, float kernel5 = -4.0f, float kernel6 = 1.0f,
		float kernel7 = 0.0f, float kernel8 = 1.0f, float kernel9 = 0.0f)
	{
		this->kernel1 = kernel1;
		this->kernel2 = kernel2;
		this->kernel3 = kernel3;
		this->kernel4 = kernel4;
		this->kernel5 = kernel5;
		this->kernel6 = kernel6;
		this->kernel7 = kernel7;
		this->kernel8 = kernel8;
		this->kernel9 = kernel9;
	}

	~laplacianSettings_t() {};
};

struct prewittSettings_t
{
	float filterLevel = 0.25f;

	GLuint bufferHandle;
	GLuint uniformHandle;

	prewittSettings_t()
	{

	}

	~prewittSettings_t() {};

};

struct freiChenSettings_t
{
	float filterLevel = 0.25f;

	GLuint bufferHandle;
	GLuint uniformHandle;

	freiChenSettings_t()
	{

	}

	~freiChenSettings_t() {};
};

class edgeDetectionScene : public texturedScene
{
public:
	//this was never finished so I'm going to leave it for last
	edgeDetectionScene(edgeFilters_t edgeFilter = SOBEL,
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (edge detection)",
		camera* edgeCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/EdgeDetection.txt")
		: texturedScene(defaultTexture, windowName, edgeCamera, shaderConfigPath)
	{
		this->currentEdgeDetection = edgeFilter;

		//don't bother with the switch. just create them all at once. only 4 types
		sobelSettingsBuffer = new sobelSettings_t();
		laplacianSettinsBuffer = new laplacianSettings_t();
		prewittSettingsBuffer = new prewittSettings_t();
		freiChenSettingsBuffer = new freiChenSettings_t();
	}

	void Initialize() override
	{
		scene::Initialize();
		this->laplacianProgramGLID = tinyShaders::GetShaderProgramByIndex(1)->handle;
		this->prewittProgramGLID = tinyShaders::GetShaderProgramByIndex(2)->handle;
		this->freiChenProgramGLID = tinyShaders::GetShaderProgramByIndex(3)->handle;
		SetupUniformBuffer<laplacianSettings_t>(laplacianSettinsBuffer, laplacianSettinsBuffer->bufferHandle, 2);
		SetupUniformBuffer<prewittSettings_t>(prewittSettingsBuffer, prewittSettingsBuffer->bufferHandle, 3);
		SetupUniformBuffer<freiChenSettings_t>(freiChenSettingsBuffer, freiChenSettingsBuffer->bufferHandle, 4);
	}

	void BuildGUI(ImGuiIO io) override
	{
		texturedScene::BuildGUI(io);

		ImGui::ListBox("edge detection types", &currentEdgeDetection, edgeDetectionSettings.data(), edgeDetectionSettings.size());
		switch (currentEdgeDetection)
		{
		case SOBEL:
		{
			//sobelSettingsBuffer = new sobelSettings_t();
			BuildSobelGUI();
			break;
		}

		case LAPLACIAN:
		{
			BuildLaplacianGUI();
			break;
		}

		case PREWITT:
		{
			BuildPrewittGUI();
			break;
		}

		case FREI_CHEN:
		{
			BuildFreiChen();
			break;
		}

		default:
			break;
		}
	}

	void BuildSobelGUI()
	{
		ImGui::Begin("sobel settings");
		ImGui::SliderFloat("red modifier", &sobelSettingsBuffer->redModifier, -1.0f, 1.0f);
		ImGui::SliderFloat("green modifier", &sobelSettingsBuffer->greenModifier, -1.0f, 1.0f);
		ImGui::SliderFloat("blue modifier", &sobelSettingsBuffer->blueModifier, -1.0f, 1.0f);
		ImGui::SliderFloat("cell distance", &sobelSettingsBuffer->cellDistance, -1.0f, 1.0f);

		ImGui::End();
	}

	void BuildLaplacianGUI()
	{
		ImGui::Begin("laplacian settings");
		ImGui::SliderFloat("kernel 1", &laplacianSettinsBuffer->kernel1, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 2", &laplacianSettinsBuffer->kernel2, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 3", &laplacianSettinsBuffer->kernel3, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 4", &laplacianSettinsBuffer->kernel4, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 5", &laplacianSettinsBuffer->kernel5, -10.0f, 10.0f);
		ImGui::SliderFloat("kernel 6", &laplacianSettinsBuffer->kernel6, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 7", &laplacianSettinsBuffer->kernel7, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 8", &laplacianSettinsBuffer->kernel8, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 9", &laplacianSettinsBuffer->kernel9, -1.0f, 1.0f);
		ImGui::SliderFloat("filter level", &laplacianSettinsBuffer->filterLevel, 0.0f, 1.0f);
		ImGui::End();
	}

	void BuildPrewittGUI()
	{
		ImGui::Begin("prewitt settings");
		ImGui::SliderFloat("offset 1", &prewittSettingsBuffer->filterLevel, 0.0f, 1.0f);
		ImGui::End();
	}

	void BuildFreiChen()
	{
		ImGui::Begin("frei-chen settings");
		ImGui::SliderFloat("offset 1", &freiChenSettingsBuffer->filterLevel, 0.0f, 1.0f);
		ImGui::End();
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		//bind appropriate GL program FIRST!!!!
		glUseProgram(programGLID);
		SetupUniformBuffer<sobelSettings_t>(sobelSettingsBuffer, sobelSettingsBuffer->bufferHandle, 1);
		SetupUniformBuffer<laplacianSettings_t>(laplacianSettinsBuffer, laplacianSettinsBuffer->bufferHandle, 2);
		SetupUniformBuffer<prewittSettings_t>(prewittSettingsBuffer, prewittSettingsBuffer->bufferHandle, 3);
		SetupUniformBuffer<freiChenSettings_t>(freiChenSettingsBuffer, freiChenSettingsBuffer->bufferHandle, 4);
		
	}

	void Update() override
	{
		scene::Update();
		switch (currentEdgeDetection)
		{
		case SOBEL:
		{
			glUseProgram(programGLID);
			UpdateUniformBuffer<sobelSettings_t>(sobelSettingsBuffer, sobelSettingsBuffer->bufferHandle);
			break;
		}

		case LAPLACIAN:
		{
			glUseProgram(laplacianProgramGLID);
			UpdateUniformBuffer<laplacianSettings_t>(laplacianSettinsBuffer, laplacianSettinsBuffer->bufferHandle);
			break;
		}

		case PREWITT:
		{
			glUseProgram(prewittProgramGLID);
			UpdateUniformBuffer<prewittSettings_t>(prewittSettingsBuffer, prewittSettingsBuffer->bufferHandle);
			break;
		}

		case FREI_CHEN:
		{
			glUseProgram(freiChenProgramGLID);
			UpdateUniformBuffer<freiChenSettings_t>(freiChenSettingsBuffer, freiChenSettingsBuffer->bufferHandle);
			break;
		}
		}
		
	}

	void Draw() override //gotta love C++ :)
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		switch (currentEdgeDetection)
		{
		case SOBEL:
		{
			glUseProgram(this->programGLID);
			break;
		}

		case LAPLACIAN:
		{
			glUseProgram(this->laplacianProgramGLID);
			break;
		}

		case PREWITT:
		{
			glUseProgram(this->prewittProgramGLID);
			break;
		}

		case FREI_CHEN:
		{
			glUseProgram(this->freiChenProgramGLID);
			break;
		}
		default:
			break;
		}

		glDrawArrays(GL_QUADS, 0, 4);

		DrawGUI(window->name);

		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

protected:

	sobelSettings_t*				sobelSettingsBuffer;
	laplacianSettings_t*			laplacianSettinsBuffer;
	prewittSettings_t*				prewittSettingsBuffer;
	freiChenSettings_t*				freiChenSettingsBuffer;
	std::vector<const char*>		edgeDetectionSettings = { "sobel", "laplacian", "prewitt", "frei chen" };
	int								currentEdgeDetection;
	GLuint							laplacianProgramGLID;
	GLuint							prewittProgramGLID;
	GLuint							freiChenProgramGLID;

};
#endif