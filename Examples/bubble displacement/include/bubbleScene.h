#ifndef BUBBLE_SCENE_H
#define BUBBLE_SCENE_H
#include <Scene.h>

class bubbleScene : public scene
{

public:

	struct bubbleSettings_t
	{
		bubbleSettings_t(GLfloat attenuation = 1.0f, GLfloat offset = 1.0f, GLuint gridDimensions = 50.0f)
		{
			this->attenuation = attenuation;
			this->offset = offset;
			this->mousePosition = mousePosition;
			this->gridDimensions = gridDimensions;
		}

		~bubbleSettings_t() {};

		glm::vec2		mousePosition;
		GLfloat			attenuation;
		GLfloat			offset;		
		GLfloat			gridDimensions;
	};

	bubbleScene(const char* windowName = "Ziyad Barakat's Portfolio ( bubble displacement )",
		camera* bubbleCamera = new camera(glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)), 
		const GLchar* shaderConfigPath = "./resources/shaders/Shaders.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f)
	{
		
		defaultUniformBuffer = new defaultUniformBuffer_t(bubbleCamera); 
		bubbleSettingsBuffer = new bubbleSettings_t();
		this->windowName = windowName;
		this->vertexArrayObject = 0;
		this->vertexBufferObject = 0;
		this->indexBufferObject = 0;

		this->sceneCamera = bubbleCamera;

		Initialize(shaderConfigPath);

		windowManager::SetWindowOnMouseMoveByIndex(0, &bubbleScene::HandleMouseMotion);
	}

	~bubbleScene( void ){}

protected:

	void CreateGrid()
	{
		GLfloat vertStepSize = defaultUniformBuffer->width / bubbleSettingsBuffer->gridDimensions;
		GLfloat horzStepSize = defaultUniformBuffer->height / bubbleSettingsBuffer->gridDimensions;

		for (GLuint rowIter = 0; rowIter < bubbleSettingsBuffer->gridDimensions; rowIter++)
		{
			for (GLuint columnIter = 0; columnIter < bubbleSettingsBuffer->gridDimensions; columnIter++)
			{
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter), (horzStepSize * rowIter), 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter) + vertStepSize, (horzStepSize * rowIter), 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter), (horzStepSize * rowIter) + horzStepSize, 1, 1));

				gridVerts.push_back(glm::vec4((vertStepSize * columnIter), (horzStepSize * rowIter) + horzStepSize, 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter) + vertStepSize, (horzStepSize * rowIter), 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter) + vertStepSize, (horzStepSize * rowIter) + horzStepSize, 1, 1));
			}
		}
	}

	void InitalizeBuffers() override
	{
		glUseProgram(this->programGLID);
		SetupUniforms();
		glEnable(GL_DEPTH_TEST);

		CreateGrid();

		glGenBuffers(1, &vertexBufferObject);
		glGenBuffers(1, &defaultUniformBufferGLID);
		glGenBuffers(1, &bubbleSettingsBufferGLID);

		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, gridVerts.size() * sizeof(glm::vec4), gridVerts.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (char*)(sizeof(float) * 4));
		
		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBufferGLID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, this->defaultSettingsHandle, this->defaultUniformBufferGLID);

		glBindBuffer(GL_UNIFORM_BUFFER, bubbleSettingsBufferGLID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(bubbleSettings_t), bubbleSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, this->bubbleSettingsHandle, this->bubbleSettingsBufferGLID);
	}

	void SetupUniforms() override
	{
		scene::SetupUniforms();
		this->bubbleSettingsHandle = glGetUniformBlockIndex(this->programGLID, "bubbleSettings");
	}

	void Draw()	override
	{
		glUseProgram(this->programGLID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(bubbleSettings_t), bubbleSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, this->bubbleSettingsHandle, this->bubbleSettingsBufferGLID);
		glDrawArrays(GL_TRIANGLES, 0, gridVerts.size());
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void HandleMouseMotion(GLuint mouseX, GLuint mouseY, GLuint screenX, GLuint screenY)
	{
		bubbleSettingsBuffer->mousePosition = glm::vec2(mouseX, mouseY);
	}

	static bubbleSettings_t*	bubbleSettingsBuffer; 
	GLuint						bubbleSettingsBufferGLID;
	GLuint						bubbleSettingsHandle;
	std::vector<glm::vec4>		gridVerts;
	GLuint						gridDimensions;
};

bubbleScene::bubbleSettings_t* bubbleScene::bubbleSettingsBuffer = nullptr;

#endif