#ifndef SCENE_H
#define SCENE_H
#define FREEIMAGE_LIB
#define TW_STATIC
//#define GLM_SWIZZLE
#include <stdlib.h>
#include <TinyExtender.h>
using namespace TinyExtender;
#include <TinyShaders.h>
#include <TinyWindow.h>
#include <TinyClock.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <matrix.hpp>
#include <vec4.hpp>
#include <gtc/matrix_transform.hpp>
#include <Camera.h>
#include <DefaultUniformBuffer.h>
#include <AntTweakBar.h>
#include <FreeImage.h>
#include "Utilities.h"
#include "VertexBuffer.h"

using namespace TinyWindow;

class scene
{
public:

	scene(const char* windowName = "Ziyad Barakat's Portfolio ( Example Scene )",
		camera* bufferCamera = new camera(),
		const GLchar* shaderConfigPath = "./shaders/Default.txt")
	{
		this->windowName = windowName;
		this->sceneCamera = bufferCamera;
		this->shaderConfigPath = shaderConfigPath;
		this->tweakBarName = windowName;

		manager = new windowManager();
		window = manager->AddWindow(windowName);
	}

	~scene(){}

	virtual void Run()
	{
		while (!window->shouldClose)
		{
			Update();
			Draw();
		}
	}

	virtual void Initialize()
	{
		FreeImage_Initialise();
		TinyExtender::InitializeExtensions();
		tinyClock::Initialize();

		tinyShaders::LoadShaderProgramsFromConfigFile(this->shaderConfigPath);
		this->programGLID = tinyShaders::GetShaderProgramByIndex(0)->handle;

		glUseProgram(this->programGLID);

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

		//this->sceneCamera->resolution = glm::vec2(width, height);

		//glEnable(GL_DEPTH_TEST);
		
		InitializeBuffers();
		SetupCallbacks();
		InitTweakBar();
	}

	virtual void SetupCallbacks()
	{
		window->SetWindowOnResizeByIndex(0, &scene::HandleWindowResize);
		window->SetWindowOnMouseButtonEventByIndex(0, &scene::HandleMouseClick);
		window->SetWindowOnMouseMoveByIndex(0, &scene::HandleMouseMotion);
		window->SetWindowOnMaximizedByIndex(0, &scene::HandleMaximize);
	}

	virtual void ShutDown() const
	{
		FreeImage_DeInitialise();
		tinyShaders::Shutdown();
		manager->ShutDown();
	}

protected:

	windowManager*							manager;
	tWindow*								window;

	static defaultUniformBuffer_t*			defaultUniformBuffer;
	static vertexBuffer_t*					defaultVertexBuffer;

	camera*									sceneCamera;
	const GLchar*							windowName;
	GLuint									programGLID;
	TwBar*									tweakBar;
	const GLchar*							tweakBarName;
	const GLchar*							shaderConfigPath;

	virtual void InitTweakBar()
	{
		TwInit(TwGraphAPI::TW_OPENGL_CORE, NULL);
		tweakBar = TwNewBar(tweakBarName);
		TwWindowSize((int)this->defaultUniformBuffer->resolution.x,
			(int)this->defaultUniformBuffer->resolution.y);

		TwAddVarRO(tweakBar, "window width", TwType::TW_TYPE_FLOAT, &defaultUniformBuffer->resolution.x, NULL);
		TwAddVarRO(tweakBar, "window height", TwType::TW_TYPE_FLOAT, &defaultUniformBuffer->resolution.y, NULL);
		TwAddVarRO(tweakBar, "mouse X", TwType::TW_TYPE_FLOAT, &defaultUniformBuffer->mousePosition.x, NULL);
		TwAddVarRO(tweakBar, "mouse Y", TwType::TW_TYPE_FLOAT, &defaultUniformBuffer->mousePosition.y, NULL);
		TwAddVarRO(tweakBar, "delta time", TwType::TW_TYPE_DOUBLE, &defaultUniformBuffer->deltaTime, NULL);
		TwAddVarRO(tweakBar, "total time", TwType::TW_TYPE_DOUBLE, &defaultUniformBuffer->totalTime, NULL);
		TwAddVarRO(tweakBar, "frames per sec", TwType::TW_TYPE_DOUBLE, &defaultUniformBuffer->framesPerSec, NULL);
		TwAddSeparator(tweakBar, NULL, NULL);
	}

	virtual void Update()
	{
		windowManager::PollForEvents();
		sceneCamera->Update();
		tinyClock::UpdateClockAdaptive();
		defaultUniformBuffer->deltaTime = tinyClock::GetDeltaTime();
		defaultUniformBuffer->totalTime = tinyClock::GetTotalTime();
		defaultUniformBuffer->framesPerSec = 1.0 / tinyClock::GetDeltaTime();
		TwRefreshBar(tweakBar);
		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
	}

	virtual void Draw()
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);

		glDrawArrays(GL_QUADS, 0, 4);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void SetupVertexBuffer()
	{

		defaultVertexBuffer = new vertexBuffer_t(defaultUniformBuffer->resolution);

		GLfloat quadVerts[16] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, sceneCamera->resolution.y, 1.0f, 1.0f,
			0.0f, sceneCamera->resolution.y, 1.0f, 1.0f
		};
	}

	/*void SetupDefaultBuffer()
	{
		glGenBuffers(1, &defaultUniformBuffer->bufferHandle);

		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, defaultUniformBuffer->bufferHandle);
	}

	void UpdateDefaultBuffer()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_STATIC_DRAW);
	}*/

	template<class Type>
	static void SetupUniformBuffer(void* buffer, GLuint& bufferHandle, GLuint bufferUniformHandle)
	{
		glGenBuffers(1, &bufferHandle);
		glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Type), buffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bufferUniformHandle, bufferHandle);
	}

	template<class Type>
	static void UpdateUniformBuffer(void* buffer, GLuint bufferHandle)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Type), buffer, GL_DYNAMIC_DRAW);
	}

	virtual void InitializeBuffers()
	{
		defaultUniformBuffer = new defaultUniformBuffer_t(this->sceneCamera);
		GLuint width = 0;
		GLuint height = 0;
		windowManager::GetWindowResolutionByIndex(0, width, height);
		glViewport(0, 0, width, height);
		glEnable(GL_DEPTH_TEST);
		defaultUniformBuffer->resolution = glm::vec2(width, height);
		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, 0.01f, 10.0f);

		//why is lower than actual resolution but still valid for tweakbar? it makes no sense!
		//set the buffer's resolution back to normal afterwards
		//SetupDefaultBuffer();
		SetupVertexBuffer();
		SetupUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle, 0);
	}

	void SetupDefaultUniforms()
	{
		//defaultUniformBuffer = new defaultUniformBuffer_t(this->sceneCamera);
		defaultUniformBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "defaultSettings");
		glUniformBlockBinding(this->programGLID, defaultUniformBuffer->uniformHandle, 0);
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

		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
		defaultVertexBuffer->UpdateBuffer(defaultUniformBuffer->resolution);
	}

	static void HandleMouseMotion(GLuint windowX, GLuint windowY, GLuint screenX, GLuint screenY)
	{
		defaultUniformBuffer->mousePosition = glm::vec2(windowX, windowY);
		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
		TwMouseMotion(windowX, windowY);
	}

	static void HandleMaximize()
	{
		GLuint width, height;
		windowManager::GetWindowResolutionByIndex(0, width, height);
		glViewport(0, 0, width, height);

		TwWindowSize(width, height);
		defaultUniformBuffer->resolution = glm::vec2(width, height);

		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, 0.01f, 10.0f);

		//bind the uniform buffer and refill it
		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_DYNAMIC_DRAW);

		//generate a new vertex buffer with the new information

		defaultVertexBuffer->UpdateBuffer(defaultUniformBuffer->resolution);		
	}
};

defaultUniformBuffer_t*		scene::defaultUniformBuffer = nullptr;
vertexBuffer_t*				scene::defaultVertexBuffer = nullptr;

#endif
