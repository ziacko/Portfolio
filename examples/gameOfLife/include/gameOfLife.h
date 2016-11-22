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

	class golSettings_t// : public uniformBuffer_t
	{
	public:
		
		glm::vec4						aliveColor;
		glm::vec4						deadColor;
		glm::vec4						emptyColor;
		float							dimensions; // change to vec2 for more flexibility

		GLuint							bufferHandle;
		GLuint							uniformHandle;

		golSettings_t(
			unsigned int dimensions = 100, glm::vec4 aliveColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), //green
			glm::vec4 deadColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), //red
			glm::vec4 emptyColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)) //blue
			//: uniformBuffer_t()
		{
			this->dimensions = (float)dimensions;

			this->aliveColor = aliveColor;
			this->deadColor = deadColor;
			this->emptyColor = emptyColor;	
		}
	};

	struct cells_t
	{
		std::vector<cellState_t>		cells;

		GLuint							bufferHandle;
		GLuint							uniformHandle;

		cells_t(golScene* gol, unsigned int dimensions)
		{
			std::srand(gol->randomSeed);
			for (GLuint cellIter = 0; cellIter < dimensions * dimensions; cellIter++)
			{
				GLuint newRand = std::rand() % 100;
				cells.push_back((cellState_t)(newRand < gol->cellProbability));
			}
		}

		~cells_t()
		{

		}
	};

	golScene(GLuint dimensions = 20, GLdouble tickDelay = 0.25f, GLuint randomSeed = 666,
		GLuint cellProbability = 90, const char* windowName = "Ziyad Barakat's portfolio (game of life)",
		camera* golCamera = new camera(), const char* shaderConfigPath = "../../resources/shaders/GOL.txt")
		: scene(windowName, golCamera, shaderConfigPath)
	{
		this->tickDelay = tickDelay;
		this->randomSeed = randomSeed;
		this->cellProbability = cellProbability;
		this->tweakBarName = windowName;

		//pass a pointer to scene just for random generator seed? sheesh past me is dumb!
		golSettingsBuffer = new golScene::golSettings_t(dimensions);
		printf("%i \n", dimensions);
		cellBuffer = new cells_t(this, dimensions);

		currentTickDelay = 0.0f;
	}

	~golScene(void) {};

protected:

	golSettings_t*						golSettingsBuffer;
	cells_t*							cellBuffer;
	GLdouble							tickDelay;
	GLdouble							currentTickDelay;
	GLuint								randomSeed;
	GLuint								cellProbability;

	void Update() override
	{
		scene::Update();
		UpdateBuffer(golSettingsBuffer, golSettingsBuffer->bufferHandle, sizeof(*golSettingsBuffer), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		UpdateBuffer(cellBuffer->cells.data(), cellBuffer->bufferHandle, sizeof(int) * cellBuffer->cells.size(), GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
/*
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, cellBuffer->bufferHandle);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * cellBuffer->cells.size(), cellBuffer->cells.data(), GL_DYNAMIC_DRAW);*/
		if (currentTickDelay < tickDelay)
		{
			currentTickDelay += sceneClock->GetDeltaTime();
		}

		else
		{
			TickOver(); //ok tickover might be the problem
			currentTickDelay = 0;
		}
	}

	void Draw() override
	{
		//UpdateBuffer(golSettingsBuffer, golSettingsBuffer->bufferHandle, sizeof(golSettingsBuffer), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		glUseProgram(this->programGLID);
		glDrawArraysInstanced(GL_QUADS, 0, 4, (GLsizei)(golSettingsBuffer->dimensions * golSettingsBuffer->dimensions));
		DrawGUI(window->name);
		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void CheckNode(cellState_t CurrentState, unsigned int& neighborCount, unsigned int& deadNeighborCount)
	{
		switch (CurrentState) // OMG that was amazing! how did i do that?
		{
		case cellState_t::EMPTY:
		{
			break;
		}
			
		case cellState_t::ALIVE:
		{
			neighborCount++;
			break;

		}
			
		case cellState_t::DEAD:
		{
			deadNeighborCount++;
			break;
		}
			
		default:
			break;
		}
	}

	void TickOver()
	{
		for (unsigned int cellIndex = 0; cellIndex < golSettingsBuffer->dimensions * golSettingsBuffer->dimensions; cellIndex++)
		{
			unsigned int column = cellIndex % (unsigned int)golSettingsBuffer->dimensions;
			unsigned int row = cellIndex & (unsigned int)golSettingsBuffer->dimensions;

			if (cellBuffer->cells[cellIndex] == EMPTY)
			{
				continue;
			}

			unsigned int neighborCount = 0;
			unsigned int deadNeighborCount = 0;
			//for convenience
			unsigned int dimensionMinOne = (unsigned int)golSettingsBuffer->dimensions - 1;

			if (cellIndex < golSettingsBuffer->dimensions)
			{
				//check if the Item is not in the last column
				if (cellIndex % (unsigned int)golSettingsBuffer->dimensions == 0)
				{
					//next row, next column
					CheckNode(cellBuffer->cells[cellIndex + dimensionMinOne + 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer->cells[cellIndex + dimensionMinOne], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer->cells[cellIndex + 1], neighborCount, deadNeighborCount);
				}

				//check if its the last column in the row
				else if (cellIndex % dimensionMinOne == 0)
				{
					//next row, last column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer->cells[cellIndex + dimensionMinOne], neighborCount, deadNeighborCount);
					//this row, last column
					CheckNode(cellBuffer->cells[cellIndex - 1], neighborCount, deadNeighborCount);
				}

				else
				{
					//5 neighbors to consider
					//this row, last column
					CheckNode(cellBuffer->cells[cellIndex - 1], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer->cells[cellIndex + 1], neighborCount, deadNeighborCount);
					//next row, last column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne)], neighborCount, deadNeighborCount);
					//next row, next column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne) + 1], neighborCount, deadNeighborCount);
				}
			}

			//check if node is in the last row
			else if (cellIndex >
				(((golSettingsBuffer->dimensions * golSettingsBuffer->dimensions) - golSettingsBuffer->dimensions) - 1)
				&& cellIndex < ((golSettingsBuffer->dimensions * golSettingsBuffer->dimensions) - 1))
			{
				//check if the column is a multiple of the dimension (the first column)
				if ((cellIndex % (unsigned int)golSettingsBuffer->dimensions) == 0)
				{
					//last row, next column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne) + 1], neighborCount, deadNeighborCount); //lower middle node
																									//last row, this column
					CheckNode(cellBuffer->cells[cellIndex - dimensionMinOne], neighborCount, deadNeighborCount); //lower middle node
																							  //this row, next column
					CheckNode(cellBuffer->cells[cellIndex + 1], neighborCount, deadNeighborCount); //lower middle node
				}

				else if ((cellIndex % dimensionMinOne == 0))
				{
					//last row, this column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne)], neighborCount, deadNeighborCount); //lower middle node
																								//last row, last column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne) - 1], neighborCount, deadNeighborCount); //lower middle node
																									//this row, last column
					CheckNode(cellBuffer->cells[cellIndex - 1], neighborCount, deadNeighborCount); //lower middle node
				}

				else
				{
					//5 neighbors to consider
					//this row, last column
					CheckNode(cellBuffer->cells[cellIndex - 1], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer->cells[cellIndex + 1], neighborCount, deadNeighborCount);
					//last row, last column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//last row, this column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne)], neighborCount, deadNeighborCount);
					//last row, next column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne) + 1], neighborCount, deadNeighborCount);
				}
			}

			else
			{
				if (cellIndex < (golSettingsBuffer->dimensions * golSettingsBuffer->dimensions) - 1)
				{
					//8 neighbors to check
					//last row, last column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//last row, this column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne)], neighborCount, deadNeighborCount);
					//last row, next column
					CheckNode(cellBuffer->cells[(cellIndex - dimensionMinOne) + 1], neighborCount, deadNeighborCount);
					//this row, last column
					CheckNode(cellBuffer->cells[cellIndex - 1], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer->cells[cellIndex + 1], neighborCount, deadNeighborCount);
					//next row, last column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne)], neighborCount, deadNeighborCount);
					//next row, next column
					CheckNode(cellBuffer->cells[(cellIndex + dimensionMinOne) + 1], neighborCount, deadNeighborCount);
				}
			}

			if (neighborCount < 2 && cellBuffer->cells[cellIndex] == ALIVE)
			{
				cellBuffer->cells[cellIndex] = DEAD;
			}

			else if (neighborCount >= 2 && cellBuffer->cells[cellIndex] == ALIVE)
			{
				if (neighborCount > 3)
				{
					cellBuffer->cells[cellIndex] = DEAD;
				}

				else
				{
					cellBuffer->cells[cellIndex] = ALIVE;
				}
			}

			else if (neighborCount == 3 && cellBuffer->cells[cellIndex] == DEAD)
			{
				cellBuffer->cells[cellIndex] = ALIVE;
			}
		}
	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(golSettingsBuffer, golSettingsBuffer->bufferHandle, sizeof(*golSettingsBuffer), 1, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		SetupBuffer(cellBuffer, cellBuffer->bufferHandle, sizeof(int) * cellBuffer->cells.size(), 0, GL_SHADER_STORAGE_BUFFER, GL_DYNAMIC_DRAW);
	}
};
