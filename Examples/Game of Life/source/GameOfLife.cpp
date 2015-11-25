#pragma once

#include <map>
#include <stdlib.h>
#include <TinyExtender.h>
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
#include <GOL_manager.h>

const GLchar* WindowName = "Ziyad Barakat's Portfolio ( Game of Life )";
GOL_manager* GOL;
camera* MainCamera;

void Draw()
{
	GOL->Draw();
	windowManager::WindowSwapBuffersByIndex(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Initialize()
{
	windowManager::Initialize();
	windowManager::AddWindow(WindowName);
	TinyExtender::InitializeExtensions();
	tinyClock::Intialize();

	MainCamera = new camera();

	tinyShaders::LoadShaderProgramsFromConfigFile("./resources/shaders/Shaders.txt");
	glViewport(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

	GOL = new GOL_manager(MainCamera, tinyShaders::GetShaderProgramByIndex(0)->handle);
	glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
}

int main ( )
{
	Initialize();

	while (!windowManager::GetWindowShouldCloseByIndex(0))
	{
		tinyClock::UpdateClockAdaptive();
		windowManager::PollForEvents();
		MainCamera->Update();

		GOL->Update(tinyClock::GetDeltaTime());

		Draw();
	}
	tinyShaders::Shutdown();
	windowManager::ShutDown();

	return 0;
};
