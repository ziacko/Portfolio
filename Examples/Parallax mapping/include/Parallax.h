#ifndef PARALLAX_H
#define PARALLAX_H

#include <Scene.h>

class parallaxScene : public scene
{
public:

	struct parallaxSettings_t
	{
		GLfloat			scale;
		GLfloat			rayHeight;
		GLuint			numSamples;

		GLuint			bufferHandle;
		GLuint			uniformHandle;

		parallaxSettings_t()
		{
			this->scale = 0.0f;
			this->rayHeight = 0.0f;
			this->numSamples = 0;
		}

		~parallaxSettings_t(){};
	};

	parallaxScene(const char* diffuseMapPath = "./textures/rocks.jpg", const char* diffuseUniformName = "diffuseMap",
		const char* heightMapPath = "./textures/rocks_NM_height.tga", const char* heightMapUniformName = "heightMap",
		const char* windowName = "Ziyad Barakat's portfolio (parallax mapping)",
		camera* parallaxCamera = new camera(glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)),
		const char* shaderConfigPath = "./shaders/Shaders.txt") : scene(windowName, parallaxCamera, shaderConfigPath, diffuseMapPath, diffuseUniformName)
	{
		this->tweakBarName = windowName;
		this->heightMapPath = heightMapPath;
		this->heightmapUniformName = heightMapUniformName;
	}

	void Initialize() override
	{
		scene::Initialize();
		diffuseMapHandle = LoadTexture(diffusePath);
		heightMapHandle = LoadTexture(heightMapPath, GL_BGRA);
	}

	~parallaxScene(){};



protected:

	static parallaxSettings_t*		parallaxSettingsBuffer;

	GLuint							heightMapHandle;
	GLuint							heightMapUniformHandle;
	const char*						heightmapUniformName;
	const char*						heightMapPath;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "parallax scale", TwType::TW_TYPE_FLOAT, &parallaxSettingsBuffer->scale, "min=0 max=10 step=0.001");
		TwAddVarRW(tweakBar, "ray height", TwType::TW_TYPE_FLOAT, &parallaxSettingsBuffer->rayHeight, "min=0 max=10 step=0.001");
		TwAddVarRW(tweakBar, "number of samples", TwType::TW_TYPE_INT16, &parallaxSettingsBuffer->numSamples, "min=0 max=1000");
	}

	void SetupParallaxBuffer()
	{
		glGenBuffers(1, &parallaxSettingsBuffer->bufferHandle);

		glBindBuffer(GL_UNIFORM_BUFFER, parallaxSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(parallaxSettings_t), parallaxSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->parallaxSettingsBuffer->bufferHandle);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		parallaxSettingsBuffer = new parallaxScene::parallaxSettings_t();
		SetupParallaxBuffer();
	}

	void SetupParallaxUniforms()
	{
		parallaxSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "parallaxSettings");
		glUniformBlockBinding(this->programGLID, parallaxSettingsBuffer->uniformHandle, 1);
	}

	void UpdateParallaxBuffer()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, parallaxSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(parallaxSettings_t), parallaxSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, parallaxSettingsBuffer->bufferHandle);
	}

	void bindTextures()	override
	{
		scene::bindTextures();
		heightMapUniformHandle = glGetUniformLocation(programGLID, heightmapUniformName);
		glUniform1i(heightMapUniformHandle, heightMapHandle);

		glActiveTexture(GL_TEXTURE0 + heightMapHandle);
		glBindTexture(GL_TEXTURE_2D, heightMapHandle);
	}

	void Draw() override
	{
		UpdateParallaxBuffer();
		glUseProgram(this->programGLID);
		bindTextures();
		glDrawArrays(GL_QUADS, 0, 4);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}


};

parallaxScene::parallaxSettings_t* parallaxScene::parallaxSettingsBuffer = nullptr;




#endif