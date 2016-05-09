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
#include <SOIL.h>
//#include "Utilities.h"
#include "VertexBuffer.h"

using namespace TinyWindow;

class scene
{
public:

	scene(const char* windowName = "Ziyad Barakat's Portfolio ( Example Scene )",
		camera* bufferCamera = new camera(),
		const GLchar* shaderConfigPath = "../../resources/shaders/Default.txt")
	{
		this->windowName = windowName;
		this->sceneCamera = bufferCamera;
		this->shaderConfigPath = shaderConfigPath;
		this->tweakBarName = windowName;

		manager = new windowManager();
		window = manager->AddWindow(windowName);
		sceneClock = new tinyClock_t();
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
		//FreeImage_Initialise();

		TinyExtender::InitializeExtentions();
		GLuint blarg = 0;
		blarg = TinyExtender::glCreateShader(gl_vertex_shader);
		tinyShaders::LoadShaderProgramsFromConfigFile(this->shaderConfigPath);
		this->programGLID = tinyShaders::GetShaderProgramByIndex(0)->handle;

		glUseProgram(this->programGLID);

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

		//this->sceneCamera->resolution = glm::vec2(width, height);

		//glEnable(GL_DEPTH_TEST);
		
		InitializeBuffers();
		SetupCallbacks();
		//InitTweakBar();
	}

	virtual void SetupCallbacks()
	{
		window->resizeEvent = &scene::HandleWindowResize;
		window->mouseButtonEvent = &scene::HandleMouseClick;
		window->mouseMoveEvent = &scene::HandleMouseMotion;
		//window->maximizedEvent = &scene::HandleMaximize;
	}

	virtual void ShutDown() const
	{
		//FreeImage_DeInitialise();
		tinyShaders::Shutdown();
		manager->ShutDown();
	}

protected:

	static windowManager*					manager;
	static tWindow*							window;
	tinyClock_t*							sceneClock;

	static defaultUniformBuffer_t*			defaultUniformBuffer;
	static vertexBuffer_t*					defaultVertexBuffer;

	camera*									sceneCamera;
	const GLchar*							windowName;
	GLuint									programGLID;
	const GLchar*							tweakBarName;
	const GLchar*							shaderConfigPath;

	/*virtual void InitTweakBar()
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
	}*/

	virtual void Update()
	{
		manager->PollForEvents();
		sceneCamera->Update();
		sceneClock->UpdateClockAdaptive();
		defaultUniformBuffer->deltaTime = sceneClock->GetDeltaTime();
		defaultUniformBuffer->totalTime = sceneClock->GetTotalTime();
		defaultUniformBuffer->framesPerSec = 1.0 / sceneClock->GetDeltaTime();
		//TwRefreshBar(tweakBar);
		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
	}

	virtual void Draw()
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);

		glDrawArrays(GL_QUADS, 0, 4);
		//TwDraw();
		window->SwapDrawBuffers();
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
		glBindBuffer(gl_uniform_buffer, bufferHandle);
		glBufferData(gl_uniform_buffer, sizeof(Type), buffer, gl_dynamic_draw);
		glBindBufferBase(gl_uniform_buffer, bufferUniformHandle, bufferHandle);
	}

	template<class Type>
	static void UpdateUniformBuffer(void* buffer, GLuint bufferHandle)
	{
		glBindBuffer(gl_uniform_buffer, bufferHandle);
		glBufferData(gl_uniform_buffer, sizeof(Type), buffer, gl_dynamic_draw);
	}

	virtual void InitializeBuffers()
	{
		defaultUniformBuffer = new defaultUniformBuffer_t(this->sceneCamera);
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		glEnable(GL_DEPTH_TEST);
		defaultUniformBuffer->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)window->resolution.width, (GLfloat)window->resolution.height, 0.0f, 0.01f, 10.0f);

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

	static void HandleMouseClick(mouseButton_t button, buttonState_t state)
	{
		//TwMouseButtonID buttonId = TwMouseButtonID::TW_MOUSE_LEFT;
		//TwMouseAction action = TwMouseAction::TW_MOUSE_PRESSED;

		if (state == buttonState_t::down)
		{
			//action = TwMouseAction::TW_MOUSE_PRESSED;
		}

		else
		{
			//action = TwMouseAction::TW_MOUSE_RELEASED;
		}

		switch (button)
		{
			case mouseButton_t::left:
			{
				//buttonId = TwMouseButtonID::TW_MOUSE_LEFT;
				break;
			}

			case mouseButton_t::right:
			{
				//buttonId = TwMouseButtonID::TW_MOUSE_RIGHT;
				break;
			}

			case mouseButton_t::middle:
			{
				//buttonId = TwMouseButtonID::TW_MOUSE_MIDDLE;
				break;
			}
		}
		//TwMouseButton(action, buttonId);
	}

	static void HandleWindowResize(TinyWindow::vec2_t<unsigned int> dimensions)
	{
		glViewport(0, 0, dimensions.width, dimensions.height);
		//TwWindowSize(dimensions.width, dimensions.height);
		defaultUniformBuffer->resolution = glm::vec2(dimensions.width, dimensions.height);
		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)dimensions.width, (GLfloat)dimensions.height, 0.0f, 0.01f, 10.0f);

		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
		defaultVertexBuffer->UpdateBuffer(defaultUniformBuffer->resolution);
	}

	static void HandleMouseMotion(vec2_t<int> windowPosition, vec2_t<int> screenPosition)
	{
		defaultUniformBuffer->mousePosition = glm::vec2(windowPosition.x, windowPosition.y);
		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
		//TwMouseMotion(windowPosition.x, windowPosition.y);
	}

	/*static void HandleMaximize()
	{
		glViewport(0, 0, window->resolution.width, window->resolution.height);

		TwWindowSize(width, height);
		defaultUniformBuffer->resolution = glm::vec2(width, height);

		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, 0.01f, 10.0f);

		//bind the uniform buffer and refill it
		glBindBuffer(gl_uniform_buffer, defaultUniformBuffer->bufferHandle);
		glBufferData(gl_uniform_buffer, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_DYNAMIC_DRAW);

		//generate a new vertex buffer with the new information

		defaultVertexBuffer->UpdateBuffer(defaultUniformBuffer->resolution);		
	}*/
};

defaultUniformBuffer_t*		scene::defaultUniformBuffer = nullptr;
vertexBuffer_t*				scene::defaultVertexBuffer = nullptr;
windowManager*				scene::manager = nullptr;
tWindow*					scene::window = nullptr;

#endif
