#ifndef PARALLAX_H
#define PARALLAX_H

#include <TexturedScene.h>

struct parallaxSettings_t
{
	GLfloat			scale;
	GLfloat			rayHeight;
	GLuint			numSamples;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	parallaxSettings_t(GLfloat scale = 0.1f, GLfloat rayHeight = 0.25f, GLuint numSamples = 100)
	{
		this->scale = scale;
		this->rayHeight = rayHeight;
		this->numSamples = numSamples;
	}

	~parallaxSettings_t(){};
};

class parallaxScene : public texturedScene
{
public:

	parallaxScene(
		parallaxSettings_t* parallaxSettings = new parallaxSettings_t(),
		texture* defaultTexture = new texture(),
		texture* heightMap = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (parallax mapping)",
		camera* parallaxCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Parallax.txt") : texturedScene(defaultTexture, windowName, parallaxCamera, shaderConfigPath)
	{
		this->parallaxSettingsBuffer = parallaxSettings;
		this->tweakBarName = windowName;
		this->heightMap = heightMap;
	}

	void Initialize() override
	{
		texturedScene::Initialize();
		heightMap->LoadTexture();
	}

	~parallaxScene(){};

protected:

	static parallaxSettings_t*		parallaxSettingsBuffer;
	texture*						heightMap;

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "parallax scale", TwType::TW_TYPE_FLOAT, &parallaxSettingsBuffer->scale, "min=0 max=10 step=0.001");
		TwAddVarRW(tweakBar, "ray height", TwType::TW_TYPE_FLOAT, &parallaxSettingsBuffer->rayHeight, "min=0 max=10 step=0.001");
		TwAddVarRW(tweakBar, "number of samples", TwType::TW_TYPE_INT16, &parallaxSettingsBuffer->numSamples, "min=0 max=1000");
	}*/

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupUniformBuffer<parallaxSettings_t>(parallaxSettingsBuffer, parallaxSettingsBuffer->bufferHandle, 1);
	}

	void SetupParallaxUniforms()
	{
		parallaxSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "parallaxSettings");
		glUniformBlockBinding(this->programGLID, parallaxSettingsBuffer->uniformHandle, 1);
	}

	void bindTextures()
	{
		defaultTexture->GetUniformLocation(programGLID); //ok so heightmap is fine. just diffuse map is screwed
		heightMap->GetUniformLocation(programGLID);
	}

	void Draw() override
	{
		UpdateUniformBuffer<parallaxSettings_t>(parallaxSettingsBuffer, parallaxSettingsBuffer->bufferHandle);
		glUseProgram(this->programGLID);
		bindTextures();
		glDrawArrays(GL_QUADS, 0, 4);
		//TwDraw();
		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


};

parallaxSettings_t* parallaxScene::parallaxSettingsBuffer = nullptr;




#endif