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
		
		glm::vec4		aliveColor;
		glm::vec4		deadColor;
		glm::vec4		emptyColor;
		float			dimensions; // change to vec2 for more flexibility

		GLuint			bufferHandle;
		GLuint			uniformHandle;

		golSettings_t(
			float dimensions, glm::vec4 aliveColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), //green
			glm::vec4 deadColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), //red
			glm::vec4 emptyColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)) //blue
		{
			this->dimensions = dimensions;

			this->aliveColor = aliveColor;
			this->deadColor = deadColor;
			this->emptyColor = emptyColor;
			uniformHandle = 0;
			bufferHandle = 0;
		}
	};

	struct cells_t
	{
		std::vector<cellState_t>		cells;

		GLuint							bufferHandle;
		GLuint							uniformHandle;

		cells_t(golScene* gol, float dimensions)
		{
			std::srand(gol->randomSeed);
			for (size_t cellIter = 0; cellIter < (size_t)dimensions * dimensions; cellIter++)
			{
				size_t newRand = std::rand() % 100;
				cells.push_back((cellState_t)(newRand < gol->cellProbability));
			}

			bufferHandle = 0;
			uniformHandle = 0;
		}

		~cells_t()
		{

		}
	};

	golScene(float dimensions = 100.0f, GLdouble tickDelay = 0.3f, GLuint randomSeed = 666,
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
		cellBuffer = new cells_t(this, dimensions);
		cellDimensions = glm::vec2(0);
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
	glm::vec2							cellDimensions;

	void SetupVertexBuffer() override
	{
		cellDimensions = glm::vec2(defaultUniform->resolution.x / golSettingsBuffer->dimensions, defaultUniform->resolution.y / golSettingsBuffer->dimensions);

		defaultVertexBuffer = new vertexBuffer_t(cellDimensions);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(golSettingsBuffer, golSettingsBuffer->bufferHandle, sizeof(*golSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);
		UpdateBuffer(cellBuffer->cells.data(), cellBuffer->bufferHandle, sizeof(int) * cellBuffer->cells.size(), gl_shader_storage_buffer, gl_dynamic_draw);
		//UpdateDefaultBuffer();

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
		glUseProgram(this->programGLID);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)(golSettingsBuffer->dimensions * golSettingsBuffer->dimensions));
		
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void CheckNode(cellState_t CurrentState, unsigned int& neighborCount, unsigned int& deadNeighborCount)
	{
		switch (CurrentState)
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
			unsigned int row = cellIndex / (unsigned int)golSettingsBuffer->dimensions;
			printf("%i \n", row);
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

	void InitializeUniforms() override 
	{
		scene::InitializeUniforms();
		SetupBuffer(golSettingsBuffer, golSettingsBuffer->bufferHandle, sizeof(*golSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
		SetupBuffer(cellBuffer, cellBuffer->bufferHandle, GLuint(sizeof(int) * cellBuffer->cells.size()), 0, gl_shader_storage_buffer, gl_dynamic_draw);
	}

	virtual void Resize(tWindow* window, glm::vec2 dimensions = glm::vec2(0)) override
	{
		glViewport(0, 0, window->resolution.x, window->resolution.y);
		defaultUniform->resolution = glm::vec2(window->resolution.x, window->resolution.y);
		defaultUniform->projection = glm::ortho(0.0f, (GLfloat)window->resolution.x, (GLfloat)window->resolution.y, 0.0f, 0.01f, 10.0f);
		cellDimensions = glm::vec2(defaultUniform->resolution.x / golSettingsBuffer->dimensions, defaultUniform->resolution.y / golSettingsBuffer->dimensions);

		UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
		if (dimensions == glm::vec2(0))
		{
			defaultVertexBuffer->UpdateBuffer(defaultUniform->resolution);
		}

		else
		{
			defaultVertexBuffer->UpdateBuffer(cellDimensions);
		}
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		Resize(window, cellDimensions);
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		Resize(window, cellDimensions);
	}
};
