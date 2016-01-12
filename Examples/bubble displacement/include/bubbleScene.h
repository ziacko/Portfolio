#ifndef BUBBLE_SCENE_H
#define BUBBLE_SCENE_H
#include <Scene.h>

class bubbleScene : public scene
{

public:

	struct bubbleSettings_t
	{
		glm::vec2		mousePosition;
		GLfloat			attenuation;
		GLfloat			offset;
		GLfloat			gridDimensions;
		GLuint			bufferHandle;
		GLuint			uniformHandle;

		bubbleSettings_t(GLfloat attenuation = 0.25f, GLfloat offset = 0.1f, GLfloat gridDimensions = 100)
		{
			this->attenuation = attenuation;
			this->offset = offset;
			this->mousePosition = glm::vec2(0);
			this->gridDimensions = gridDimensions;
		}

		~bubbleSettings_t() {};
	};

	bubbleScene(const char* windowName = "Ziyad Barakat's Portfolio ( bubble displacement )",
		const char* texturePath = "./textures/earth_diffuse.tga",
		camera* bubbleCamera = new camera(glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)), 
		const GLchar* shaderConfigPath = "./shaders/Shaders.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f)
	{
		
		defaultUniformBuffer = new defaultUniformBuffer_t(bubbleCamera); 
		bubbleSettingsBuffer = new bubbleSettings_t();
		this->windowName = windowName;
		this->vertexArrayObject = 0;
		this->vertexBufferObject = 0;
		this->indexBufferObject = 0;

		this->sceneCamera = bubbleCamera;
		this->tweakBarName = "bubble scene";		

		Initialize(shaderConfigPath);
		GLuint width = 0;
		GLuint height = 0;

		windowManager::GetWindowResolutionByIndex(0, width, height);
		glViewport(0, 0, width, height);
		TwWindowSize(width, height);

		//load the texture
		textureHandle = LoadTexture(texturePath, GL_BGRA);
		//textureHandle = SOIL_load_OGL_texture(texturePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, NULL);

		windowManager::SetWindowOnMouseMoveByIndex(0, &bubbleScene::HandleMouseMotion);
		windowManager::SetWindowOnResizeByIndex(0, &bubbleScene::HandleWindowResize);
	}

	~bubbleScene( void ){}

protected:

	/*void CreateGrid()
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
	}*/

	static bubbleSettings_t*	bubbleSettingsBuffer;
	std::vector<glm::vec4>		gridVerts;
	GLuint						gridDimensions;

	void Initialize(const char* shaderConfigPath) override
	{
		scene::Initialize(shaderConfigPath);
		windowManager::SetWindowOnMouseMoveByIndex(0, &bubbleScene::HandleMouseMotion);
		windowManager::SetWindowOnMouseButtonEventByIndex(0, &bubbleScene::HandleMouseClick);
		InitTweakBar();
	}

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRO(tweakBar, "mouse Position X", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->mousePosition.x, NULL);
		TwAddVarRO(tweakBar, "mouse position Y", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->mousePosition.y, NULL);
		TwAddVarRW(tweakBar, "attenuation", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->attenuation, "min=0.01 max=1 step=0.01");
		TwAddVarRW(tweakBar, "offset", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->offset, "min=-1 max=1 step=0.01");
		TwAddVarRW(tweakBar, "grid dimensions", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->gridDimensions, "min=1 max=1000");
	}

	void SetupBubbleBuffer()
	{
		glGenBuffers(1, &bubbleSettingsBuffer->bufferHandle);
		glBindBuffer(GL_UNIFORM_BUFFER, bubbleSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(bubbleSettings_t), bubbleSettingsBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, this->bubbleSettingsBuffer->bufferHandle);
	}

	void InitializeBuffers() override
	{
		//SetupVertexBuffer();
		scene::SetupDefaultBuffer();
		SetupBubbleBuffer();

	}

	void SetupVertexBuffer() override
	{
		GLfloat cellWidth = defaultUniformBuffer->resolution.x / bubbleSettingsBuffer->gridDimensions;
		GLfloat cellHeight = defaultUniformBuffer->resolution.y / bubbleSettingsBuffer->gridDimensions;

		GLfloat quadVerts[] =
		{
			0.0f, 0.0f, 0.1f, 1.0f,
			cellWidth, 0.0f, 0.1f, 1.0f,
			cellWidth, cellHeight, 0.1f, 1.0f,
			0.0f, cellHeight, 0.1f, 1.0f
		};

		glEnable(GL_DEPTH_TEST);

		glGenBuffers(1, &vertexBufferObject);
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quadVerts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (char*)(sizeof(float) * 4));
	}

	void SetupBubbleUniforms()
	{
		textureUniformHandle = glGetUniformLocation(programGLID, "bubbleTexture");
		//bubbleSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "bubbleSettings");
		//glUniformBlockBinding(this->programGLID, this->bubbleSettingsBuffer->uniformHandle, 1);
	}

	void Draw()	override
	{
		glUniform1i(textureUniformHandle, textureHandle);
		glActiveTexture(GL_TEXTURE0 + textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glUseProgram(this->programGLID);
		
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)(this->bubbleSettingsBuffer->gridDimensions * this->bubbleSettingsBuffer->gridDimensions));
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	static void HandleMouseMotion(GLuint windowX, GLuint windowY, GLuint screenX, GLuint screenY)
	{
		//if the window is maximized use the screen mouse position?
		if (windowManager::GetWindowIsMaximizedByIndex(0))
		{
			bubbleSettingsBuffer->mousePosition = glm::vec2(screenX, screenY);
			TwMouseMotion(screenX, screenY);
		}

		else
		{
			bubbleSettingsBuffer->mousePosition = glm::vec2(windowX, windowY);
			TwMouseMotion(windowX, windowY);
		}

		glBindBuffer(GL_UNIFORM_BUFFER, bubbleSettingsBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(bubbleSettings_t), bubbleSettingsBuffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 1, bubbleSettingsBuffer->bufferHandle);
		
	}

	static void HandleMouseClick(GLuint button, GLboolean buttonState)
	{
		TwMouseButtonID buttonId = TwMouseButtonID::TW_MOUSE_LEFT;
		TwMouseAction action = TwMouseAction::TW_MOUSE_PRESSED;

		if (buttonState == 1)
		{
			action = TwMouseAction::TW_MOUSE_PRESSED;
		}

		else
		{
			action = TwMouseAction::TW_MOUSE_RELEASED;
		}

		switch (button)
		{
			case MOUSE_LEFTBUTTON:
			{
				buttonId = TwMouseButtonID::TW_MOUSE_LEFT;
				break;
			}

			case MOUSE_RIGHTBUTTON:
			{
				buttonId = TwMouseButtonID::TW_MOUSE_RIGHT;
				break;
			}

			case MOUSE_MIDDLEBUTTON:
			{
				buttonId = TwMouseButtonID::TW_MOUSE_MIDDLE;
				break;
			}
		}
		TwMouseButton(action, buttonId);
	}

	static void HandleWindowResize(GLuint width, GLuint height)
	{
		glViewport(0, 0, width, height);
		TwWindowSize(width, height);
		defaultUniformBuffer->resolution = glm::vec2(width, height);

		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, 0.01f, 10.0f);

		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, defaultUniformBuffer->bufferHandle);

		//update the projection matrix in the buffer

	}

	
};

bubbleScene::bubbleSettings_t* bubbleScene::bubbleSettingsBuffer = nullptr;

#endif