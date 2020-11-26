#pragma once

#include <Scene.h>
#include <cstdlib>
#include "../../gameOfLife/include/gameOfLife.h"

class GOLCompute : public golScene
{
public:

	GOLCompute(const char* windowName = "Ziyad Barakat's portfolio (game of life (compute)",
		camera* golCamera = new camera(), const char* shaderConfigPath = "../../resources/shaders/GOLComp.txt")
		: golScene(32.0f, 0.3, 666, 90, windowName, golCamera, shaderConfigPath)
	{

	}

	~GOLCompute(void) {};

	void Initialize() override
	{
		golScene::Initialize();
		computeProgram = shaderPrograms[1]->handle;
	}

protected:

	unsigned int computeProgram = 0;

	void Update() override
	{
		scene::Update();
		/*gol.Update();
		cellBuffer.Update(gl_shader_storage_buffer, gl_dynamic_draw, sizeof(int) * cellBuffer.data.cells.size(), cellBuffer.data.cells.data());*/

		if (currentTickDelay < tickDelay)
		{
			currentTickDelay += sceneClock->GetDeltaTime();
		}

		else
		{
			glUseProgram(computeProgram);
			glDispatchCompute(25, 25, 1);
			currentTickDelay = 0;
		}
	}

	void InitializeUniforms() override
	{
		golScene::InitializeUniforms();
		cellBuffer.Update(gl_shader_storage_buffer, gl_dynamic_draw, sizeof(int) * cellBuffer.data.cells.size(), cellBuffer.data.cells.data());
	}
};
