#ifndef PARALLAX_SCENE_H
#define PARALLAX_SCENE_H
#include <Scene.h>

class parallaxScene : public scene
{
public:

	struct parallaxSettings_t
	{

		glm::vec2		mousePosition;
		GLfloat			parallaxScale;
		GLfloat			scaleBias;
		GLfloat			rayHeight;
		GLuint			numSamples;
		GLuint			diffuseMap;
		GLuint			heightMap;

		parallaxSettings_t(GLuint diffuseMap, GLuint heightMap, GLfloat parallaxScale = 1.0f, 
			GLfloat scaleBias = 1.0f, GLfloat rayHeight = 1.0f, GLuint numSamples = 100)
		{
			this->diffuseMap = diffuseMap; 
			this->heightMap = heightMap;
			this->parallaxScale = parallaxScale;
			this->scaleBias = scaleBias;
			this->rayHeight = rayHeight;
			this->numSamples = numSamples;
		}

		~parallaxSettings_t(){};


	};

	static parallaxSettings_t*		parallaxSettingsBuffer;
	GLuint							parallaxSettingsBufferGLID;
	GLuint							parallaxSettingsHandle;

	parallaxScene(){};

	parallaxScene(GLuint diffuseMap, GLuint heightMap, const GLchar* windowName = "Ziyad Barakat's Portfolio (parallax mapping)",
		camera* parallaxCamera = new camera(glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)),
		const GLchar* shaderConfigPath = "./shaders/Shaders.txt", GLfloat parallaxScale = 1.0f,
		GLfloat scaleBias = 1.0f, GLfloat rayHeight = 1.0f, GLuint numSamples = 100)
	{
		defaultUniformBuffer = new defaultUniformBuffer_t(parallaxCamera);
		parallaxSettingsBuffer = new parallaxSettings_t(diffuseMap, heightMap, parallaxScale, scaleBias, rayHeight, numSamples);
		this->windowName = windowName;
		this->vertexArrayObject = 0;
		this->vertexBufferObject = 0;
		this->indexBufferObject = 0;

		this->sceneCamera = parallaxCamera;

		Initialize(shaderConfigPath);

		//windowManager::SetWindowOnMouseMoveByIndex(0, &scene::HandleMouseMotion);

	}

	void Initialize(const char* shaderConfigPath) override
	{
		scene::Initialize(shaderConfigPath);
	}

	void SetupParallaxBuffer()
	{
		glGenBuffers(1, &parallaxSettingsBufferGLID);

		glBindBuffer(GL_UNIFORM_BUFFER, parallaxSettingsBufferGLID);
		SetupParallaxUniforms();
		glBufferData(GL_UNIFORM_BUFFER, sizeof(parallaxSettings_t), parallaxSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, this->parallaxSettingsHandle, this->parallaxSettingsBufferGLID);
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupParallaxBuffer();


	}

	void SetupParallaxUniforms()
	{
		this->parallaxSettingsHandle = glGetUniformBlockIndex(this->programGLID, "parallaxSettings");
		glUniformBlockBinding(this->programGLID, this->parallaxSettingsHandle, 1);
	}

	
};
parallaxScene::parallaxSettings_t* parallaxScene::parallaxSettingsBuffer = nullptr;


#endif
