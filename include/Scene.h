#ifndef SCENE_H
#define SCENE_H

#include <stdlib.h>
#include <TinyExtender.h>
#include <TinyShaders.h>
#include <TinyWindow.h>
#include <tinyclock.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <matrix.hpp>
#include <vec4.hpp>
#include <gtc/matrix_transform.hpp>
#include <Camera.h>
#include <DefaultUniformBuffer.h>

class scene
{
public:
	scene(){}
	~scene(){}

	virtual void Run()
	{
		while (!windowManager::GetWindowShouldCloseByIndex(0))
		{
			Update();
			Draw();
		}
	}

protected:

	defaultUniformBuffer_t*			defaultUniformBuffer;
	GLuint							defaultUniformBufferGLID;
	GLuint							defaultSettingsHandle;

	GLuint							vertexArrayObject;
	GLuint							vertexBufferObject;
	GLuint							indexBufferObject;
	camera*							sceneCamera;
	const GLchar*					windowName;
	GLuint							programGLID;

	virtual void Initialize(const GLchar* shaderConfigPath)
	{
		windowManager::Initialize();
		windowManager::AddWindow(windowName);
		TinyExtender::InitializeExtensions();
		tinyClock::Intialize();

		tinyShaders::LoadShaderProgramsFromConfigFile(shaderConfigPath);
		this->programGLID = tinyShaders::GetShaderProgramByIndex(0)->handle;

		InitalizeBuffers();
		glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void Update()
	{
		windowManager::PollForEvents();
		sceneCamera->Update();
	}

	virtual void Draw()
	{
		glBindVertexArray(this->vertexArrayObject);
		glUseProgram(this->programGLID);

		glDrawArrays(GL_QUADS, 0, 4);
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void InitalizeBuffers()
	{
		GLfloat quadVerts[] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, sceneCamera->resolution.y, 1.0f, 1.0f,
			0.0f, sceneCamera->resolution.y, 1.0f, 1.0f
		};

		//glEnable(GL_DEPTH_TEST);

		glGenBuffers(1, &vertexBufferObject);
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quadVerts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (char*)(sizeof(float) * 4));

		glGenBuffers(1, &defaultUniformBufferGLID);

		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBufferGLID);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_STATIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, this->defaultSettingsHandle, this->defaultUniformBufferGLID);		
	}

	virtual void SetupUniforms()
	{
		this->defaultSettingsHandle = glGetUniformBlockIndex(this->programGLID, "defaultSettings");
	}

	virtual void ShutDown() const
	{
		tinyShaders::Shutdown();
		windowManager::ShutDown();
	}
};

#endif