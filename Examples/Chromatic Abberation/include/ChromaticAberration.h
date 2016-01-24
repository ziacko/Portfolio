#ifndef CHROMATICABERRATION_H
#define CHROMATICABERRATION_H
#include "Scene.h"

class chromaticScene : public scene
{

public:

	struct chromaticSettings_t
	{
		GLfloat			redOffset;
		GLfloat			blueOffset;
		GLfloat			greenOffset;

		GLuint			bufferHandle;
		GLuint			uniformHandle;

		chromaticSettings_t()
		{
			this->redOffset = 0.0f;
			this->greenOffset = 0.0f;
			this->blueOffset = 0.0f;
		}

		~chromaticSettings_t(){};
	};

	chromaticScene(const char* windowName = "Ziyad Barakat's portfolio (chromatic aberration)",
		camera* chromaticCamera = new camera(glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)),
		const char* shaderConfigPath = "./shaders/Shaders.txt") : scene(windowName, chromaticCamera, shaderConfigPath)
	{
		this->tweakBarName = windowName;

		
	}

	void Initialize() override
	{
		scene::Initialize();
		diffuseMapHandle = LoadTexture(diffusePath, GL_BGRA);
	}

	~chromaticScene( void ){}

protected:

	static chromaticSettings_t*		chromaticSettingsBuffer;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "red offset", TwType::TW_TYPE_FLOAT, &chromaticSettingsBuffer->redOffset, "min=0 max=10 step=0.001");
		TwAddVarRW(tweakBar, "green offset", TwType::TW_TYPE_FLOAT, &chromaticSettingsBuffer->greenOffset, "min=0 max=10 step=0.001");
		TwAddVarRW(tweakBar, "blue offset", TwType::TW_TYPE_FLOAT, &chromaticSettingsBuffer->blueOffset, "min=0 max=10 step=0.001");
	}

	void SetupChromaticBuffer()
	{
		glGenBuffers(1, &chromaticSettingsBuffer->bufferHandle);

		glBindBuffer(GL_UNIFORM_BUFFER, chromaticSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(chromaticSettings_t), chromaticSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->chromaticSettingsBuffer->bufferHandle);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		chromaticSettingsBuffer = new chromaticSettings_t();
		SetupChromaticBuffer();
	}

	void SetupChromaticUniforms()
	{
		chromaticSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "chromaticSettings");
		glUniformBlockBinding(this->programGLID, chromaticSettingsBuffer->uniformHandle, 1);
	}

	void UpdateChromaticBuffer()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, chromaticSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(chromaticSettings_t), chromaticSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, chromaticSettingsBuffer->bufferHandle);
	}

	void Draw() override
	{
		UpdateChromaticBuffer();
		glUseProgram(this->programGLID);
		bindTextures();
		glDrawArrays(GL_QUADS, 0, 4);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

};

chromaticScene::chromaticSettings_t* chromaticScene::chromaticSettingsBuffer = nullptr;


#endif