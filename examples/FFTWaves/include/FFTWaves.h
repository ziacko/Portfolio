#ifndef FFTWAVES_H
#define FFTWAVES_H
#include "../../include/Scene3D.h"
#include "Model.h"
#include "Grid.h"
#include "FrameBuffer.h"
#include "FFTransform.h"
#include "WaveCascade.h"

class FFTWaves : public scene3D
{
public:

	FFTWaves(const char* windowName = "Ziyad Barakat's Portfolio(FFT waves)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 100.0f, camera::projection_t::perspective),
		model_t* model = new model_t(),
		const char* shaderConfigPath = "../../resources/shaders/Model.txt") : 
		scene3D(windowName, camera3D, shaderConfigPath, model)
	{
		testModel = model;
	}

	~FFTWaves() {};

	virtual void Initialize() override
	{
		scene::Initialize();
		drawGrid = new grid(glm::ivec2(100, 100));
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

protected:

	grid* drawGrid;

	virtual void Draw() override 
	{
		glBindVertexArray(drawGrid->vertexArrayHandle);
		glUseProgram(this->programGLID);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		drawGrid->Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	virtual void Update() override
	{
		//this keeps resetting the values
		manager->PollForEvents();
		sceneCamera->Update();
		sceneClock->UpdateClockAdaptive();

		defaultPayload.data.deltaTime = (float)sceneClock->GetDeltaTime();
		defaultPayload.data.totalTime = (float)sceneClock->GetTotalTime();
		defaultPayload.data.framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultPayload.data.totalFrames++;

		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.view = sceneCamera->view;
		if (sceneCamera->currentProjectionType == camera::projection_t::perspective)
		{
			defaultPayload.data.translation = testModel->makeTransform();
		}

		else
		{
			defaultPayload.data.translation = sceneCamera->translation;
		}

		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void InitializeUniforms() override
	{
		defaultPayload.data = defaultUniformBuffer(sceneCamera);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		defaultPayload.data.resolution = glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.translation = sceneCamera->translation;
		defaultPayload.data.view = sceneCamera->view;

		materialBuffer.data = baseMaterialSettings_t();

		defaultPayload.Initialize(0);
		materialBuffer.Initialize(1);
	}
};

#endif

