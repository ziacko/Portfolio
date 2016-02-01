#ifndef PERLINNOISE_H
#define PERLINNOISE_H
#include "Scene.h"

struct perlinSettings_t//	: public uniformBuffer_t
{
	GLfloat			modValue;
	GLfloat			permuteValue;
	GLfloat			taylorInverse;
	GLfloat			fadeValue1;
	GLfloat			fadeValue2;
	GLfloat			fadeValue3;
	GLuint			numOctaves;

	GLfloat			noiseValue;
	GLfloat			noiseValue2;
	GLuint			colorBias;//20

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

	GLfloat			pattern2Value1;//0.3
	GLfloat			pattern2Value2;//0.8
	GLfloat			pattern2Value3;//5.2
	GLfloat			pattern2Value4;//1.3
	GLfloat			pattern2Value5;//4.0
	GLfloat			pattern2Value6;//1.7
	GLfloat			pattern2Value7;//9.2
	GLfloat			pattern2Value8;//4.0
	GLfloat			pattern2Value9;//8.3
	GLfloat			pattern2Value10;//2.8
	GLfloat			pattern2Value11;//4.0

	GLuint			bufferHandle;
	GLuint			uniformHandle;

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

		this->uniformHandle = 0;
		this->bufferHandle = 0;
	};

	~perlinSettings_t(){};
};

class perlinScene : public scene
{
public:

	perlinScene(
		perlinSettings_t* perlinSettings = new perlinSettings_t(),
		const char* windowName = "Ziyad Barakat's Portfolio ( Perlin noise )",
		camera* perlinCamera = new camera(), const GLchar* shaderConfigPath = "./shaders/Perlin.txt")
		: scene(windowName, perlinCamera, shaderConfigPath)
	{
		this->perlinSettingsBuffer = perlinSettings;
		tweakBarName = windowName;
	}

	void Initialize() override
	{
		scene::Initialize();
	}

protected:

	static perlinSettings_t*		perlinSettingsBuffer;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "mod Value", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->modValue, "min=0.01 max=1000 step=0.1");
		TwAddVarRW(tweakBar, "permute Value", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->permuteValue, "min=0.01 max=100 step=0.1");
		TwAddVarRW(tweakBar, "taylor inverse", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->taylorInverse, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "fade value 1", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->fadeValue1, "min=0.01 max=100 step=0.01");
		TwAddVarRW(tweakBar, "fade value 2", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->fadeValue2, "min=0.01 max=100 step=0.01");
		TwAddVarRW(tweakBar, "fade value 3", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->fadeValue3, "min=0.01 max=100 step=0.01");
		TwAddVarRW(tweakBar, "num octaves", TwType::TW_TYPE_INT16, &perlinSettingsBuffer->numOctaves, "min=0 max=100 step=1");
		
		TwAddSeparator(tweakBar, "main settings", NULL);

		TwAddVarRW(tweakBar, "noise value 1", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->noiseValue, "min=0.01 max=100 step=0.01");
		TwAddVarRW(tweakBar, "noise value 2", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->noiseValue2, "min=0.01 max=100 step=0.01");
		TwAddVarRW(tweakBar, "color bias", TwType::TW_TYPE_INT16, &perlinSettingsBuffer->colorBias, "min=0 max=100 step=1");

		TwAddSeparator(tweakBar, "secondary settings", NULL);

		/*TwAddVarRW(tweakBar, "pattern 1 value 1", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value1, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 2", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value2, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 3", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value3, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 4", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value4, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 5", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value5, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 6", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value6, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 7", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value7, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 8", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value8, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 9", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value9, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 10", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value10, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern 1 value 11", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern1Value11, "min=0.01 max=10 step=0.01");	*/

		//TwAddSeparator(tweakBar, "tertiary settings", NULL);

		TwAddVarRW(tweakBar, "pattern value 1", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value1, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 2", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value2, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 3", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value3, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 4", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value4, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 5", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value5, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 6", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value6, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 7", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value7, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 8", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value8, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 9", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value9, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 10", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value10, "min=0.01 max=10 step=0.01");
		TwAddVarRW(tweakBar, "pattern value 11", TwType::TW_TYPE_FLOAT, &perlinSettingsBuffer->pattern2Value11, "min=0.01 max=10 step=0.01");
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<perlinSettings_t>(perlinSettingsBuffer, perlinSettingsBuffer->bufferHandle, 1);
	}

	void SetPerlinUniforms()
	{
		perlinSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "perlinSettings");
		glUniformBlockBinding(this->programGLID, perlinSettingsBuffer->uniformHandle, 1);
	}

	void Draw()	override
	{
		UpdateUniformBuffer<perlinSettings_t>(perlinSettingsBuffer, perlinSettingsBuffer->bufferHandle);
		glUseProgram(this->programGLID);
		glDrawArrays(GL_QUADS, 0, 4);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

perlinSettings_t* perlinScene::perlinSettingsBuffer = nullptr;
#endif