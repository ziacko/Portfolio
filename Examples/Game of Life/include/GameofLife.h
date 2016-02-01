#include <Scene.h>
#include <stdlib.h>
#include "UniformBuffer.h"

class golScene : public scene
{
public:

	enum cellState_t
	{
		EMPTY = 0,
		ALIVE,
		DEAD
	};

	struct golSettings_t// : public uniformBuffer_t
	{
		GLuint							dimensions;
		glm::vec4						aliveColor;
		glm::vec4						deadColor;
		glm::vec4						emptyColor;

		std::vector<cellState_t>		cells;

		GLuint							bufferHandle;
		GLuint							uniformHandle;

		golSettings_t(golScene* gol,
			GLuint dimensions = 100, glm::vec4 aliveColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
			glm::vec4 deadColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
			glm::vec4 emptyColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
		{
			this->dimensions = dimensions;

			this->aliveColor = aliveColor;
			this->deadColor = deadColor;
			this->emptyColor = emptyColor;

			std::srand(gol->randomSeed);
			for (GLuint cellIter = 0; cellIter < dimensions * dimensions; cellIter++)
			{
				GLuint newRand = std::rand() % 100;
				cells.push_back((cellState_t)(newRand < gol->cellProbability));
			}

			//GenerateBuffers();
		}
	};

	golScene(GLdouble tickDelay = 0.5, GLuint randomSeed = 1337,
		GLuint cellProbability = 90, const char* windowName = "Ziyad Barakat's portfolio (game of life)",
		camera* golCamera = new camera(), const char* shaderConfigPath = "./shaders/GOL.txt")
		: scene(windowName, golCamera, shaderConfigPath)
	{
		this->tickDelay = tickDelay;
		this->randomSeed = randomSeed;
		this->cellProbability = cellProbability;
		this->tweakBarName = windowName;
	}

	~golScene(void) {};

protected:

	static golSettings_t*				golSettingsBuffer;
	GLdouble							tickDelay;
	GLdouble							currentTickDelay;
	GLuint								randomSeed;
	GLuint								cellProbability;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
	}

	void Update() override
	{
		scene::Update();

		if (currentTickDelay < tickDelay)
		{
			currentTickDelay += tinyClock::GetDeltaTime();
		}

		else
		{
			currentTickDelay = 0;
		}
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		golSettingsBuffer = new golScene::golSettings_t(this);
		SetupUniformBuffer<golSettings_t>(golSettingsBuffer, golSettingsBuffer->bufferHandle, 1);

	}

	void Draw() override
	{
		UpdateUniformBuffer<golSettings_t>(golSettingsBuffer, golSettingsBuffer->bufferHandle);
		glUseProgram(this->programGLID);
		glDrawArraysInstanced(GL_QUADS, 0, 4, (GLsizei)(golSettingsBuffer->dimensions * golSettingsBuffer->dimensions));
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

golScene::golSettings_t* golScene::golSettingsBuffer = nullptr;