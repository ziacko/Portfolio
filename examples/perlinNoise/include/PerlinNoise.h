#ifndef PERLINNOISE_H
#define PERLINNOISE_H
#include "Scene.h"

struct perlinSettings_t//	: public uniformBuffer_t
{
	float			modValue;
	float			permuteValue;
	float			taylorInverse;
	float			fadeValue1;
	float			fadeValue2;
	float			fadeValue3;
	int				numOctaves;

	float			noiseValue;
	float			noiseValue2;
	int				colorBias;//20

	/*
	GLfloat			pattern1Value1;//2.5
	GLfloat			pattern1Value2;//0.4
	GLfloat			pattern1Value3;//5.2
	GLfloat			pattern1Value4;//1.3
	GLfloat			pattern1Value5;//4.0
	GLfloat			pattern1Value6;//1.7
	GLfloat			pattern1Value7;//9.2
	GLfloat			pattern1Value8;//4.0
	GLfloat			pattern1Value9;//8.3
	GLfloat			pattern1Value10;//2.8
	GLfloat			pattern1Value11;//4.0*/

	float			pattern2Value1;//0.3
	float			pattern2Value2;//0.8
	float			pattern2Value3;//5.2
	float			pattern2Value4;//1.3
	float			pattern2Value5;//4.0
	float			pattern2Value6;//1.7
	float			pattern2Value7;//9.2
	float			pattern2Value8;//4.0
	float			pattern2Value9;//8.3
	float			pattern2Value10;//2.8
	float			pattern2Value11;//4.0

	perlinSettings_t(GLfloat modValue = 289.0f, GLfloat permuteValue = 34.0f,
		GLfloat taylorInverse = 1.79284291400159f, GLfloat fadeValue1 = 6.0f, GLfloat fadeValue2 = 15.0f,
		GLfloat fadeValue3 = 10.0f, GLuint numOctaves = 4,
		GLuint colorBias = 20,
		GLfloat noiseValue = 41.0f, GLfloat noiseValue2 = 2.3f,

		GLfloat pattern1Value1 = 2.5f, GLfloat pattern1Value2 = 0.4f, GLfloat pattern1Value3 = 5.2f,
		GLfloat pattern1Value4 = 1.3f, GLfloat pattern1Value5 = 4.0f, GLfloat pattern1Value6 = 1.7f,
		GLfloat pattern1Value7 = 9.2f, GLfloat pattern1Value8 = 4.0f, GLfloat pattern1Value9 = 8.3f,
		GLfloat pattern1Value10 = 2.8f, GLfloat	pattern1Value11 = 4.0f,

		GLfloat pattern2Value1 = 0.3f, GLfloat pattern2Value2 = 0.8f, GLfloat pattern2Value3 = 5.2f,
		GLfloat pattern2Value4 = 1.3f, GLfloat pattern2Value5 = 4.0f, GLfloat pattern2Value6 = 1.7f,
		GLfloat pattern2Value7 = 9.2f, GLfloat pattern2Value8 = 4.0f, GLfloat pattern2Value9 = 8.3f,
		GLfloat pattern2Value10 = 2.8f, GLfloat	pattern2Value11 = 4.0f)
	{
		this->modValue = modValue;
		this->permuteValue = permuteValue;
		this->taylorInverse = taylorInverse;
		this->fadeValue1 = fadeValue1;
		this->fadeValue2 = fadeValue2;
		this->fadeValue3 = fadeValue3;
		this->numOctaves = numOctaves;

		this->colorBias = colorBias;

		this->noiseValue = noiseValue;
		this->noiseValue2 = noiseValue2;

		/*this->pattern1Value1 = pattern1Value1;
		this->pattern1Value2 = pattern1Value2;
		this->pattern1Value3 = pattern1Value3;
		this->pattern1Value4 = pattern1Value4;
		this->pattern1Value5 = pattern1Value5;
		this->pattern1Value6 = pattern1Value6;
		this->pattern1Value7 = pattern1Value7;
		this->pattern1Value8 = pattern1Value8;
		this->pattern1Value9 = pattern1Value9;
		this->pattern1Value10 = pattern1Value10;
		this->pattern1Value11 = pattern1Value11;*/

		this->pattern2Value1 = pattern2Value1;
		this->pattern2Value2 = pattern2Value2;
		this->pattern2Value3 = pattern2Value3;
		this->pattern2Value4 = pattern2Value4;
		this->pattern2Value5 = pattern2Value5;
		this->pattern2Value6 = pattern2Value6;
		this->pattern2Value7 = pattern2Value7;
		this->pattern2Value8 = pattern2Value8;
		this->pattern2Value9 = pattern2Value9;
		this->pattern2Value10 = pattern2Value10;
		this->pattern2Value11 = pattern2Value11;
	};

	~perlinSettings_t(){};
};

class perlinScene : public scene
{
public:

	perlinScene(
		bufferHandler_t<perlinSettings_t> perlinSettings = bufferHandler_t<perlinSettings_t>(),
		const char* windowName = "Ziyad Barakat's Portfolio ( Perlin noise )",
		camera* perlinCamera = new camera(), const GLchar* shaderConfigPath = "../../resources/shaders/Perlin.txt")
		: scene(windowName, perlinCamera, shaderConfigPath)
	{
		this->perlin = perlinSettings;
		tweakBarName = windowName;
	}

	void Initialize() override
	{
		scene::Initialize();
	}

protected:

	bufferHandler_t<perlinSettings_t>		perlin;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene::BuildGUI(window, io); 

		ImGui::SliderFloat("modifier value", &perlin.data.modValue, 0.0f, 1000.0f);
		ImGui::SliderFloat("permutation value", &perlin.data.permuteValue, 0.0f, 100.0f);
		ImGui::SliderFloat("taylor inverse", &perlin.data.taylorInverse, 0.0f, 10.0f);

		AddGUISpacer();

		ImGui::SliderFloat("fade value 1", &perlin.data.fadeValue1, 0.0f, 100.0f);
		ImGui::SliderFloat("fade value 2", &perlin.data.fadeValue2, 0.0f, 100.0f);
		ImGui::SliderFloat("fade value 3", &perlin.data.fadeValue3, 0.0f, 100.0f);

		AddGUISpacer();

		ImGui::SliderInt("num octaves", &perlin.data.numOctaves, 0, 100);
		
		AddGUISpacer();

		ImGui::SliderInt("color bias", &perlin.data.colorBias, 0, 100);

		AddGUISpacer();
		ImGui::SliderFloat("pattern value 1", &perlin.data.pattern2Value1, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 2", &perlin.data.pattern2Value2, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 3", &perlin.data.pattern2Value3, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 4", &perlin.data.pattern2Value4, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 5", &perlin.data.pattern2Value5, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 6", &perlin.data.pattern2Value6, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 7", &perlin.data.pattern2Value7, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 8", &perlin.data.pattern2Value8, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 9", &perlin.data.pattern2Value9, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 10", &perlin.data.pattern2Value10, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 11", &perlin.data.pattern2Value11, 0.0f, 10.0f);
	}

	void AddGUISpacer()
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		perlin.Initialize(1);
	}

	void SetPerlinUniforms()
	{
		perlin.SetupUniforms(this->programGLID, "perlinSettings", 1);
	}

	void Update() override
	{
		scene::Update();
		perlin.Update();

	}

	void Draw()	override
	{
		glUseProgram(this->programGLID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
#endif
