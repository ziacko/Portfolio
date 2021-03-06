#pragma once

#include <Scene.h>
#include <cstdlib>
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

		golSettings_t(
			float dimensions = 10, glm::vec4 aliveColor = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), //green
			glm::vec4 deadColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), //red
			glm::vec4 emptyColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)) //blue
		{
			this->dimensions = dimensions;

			this->aliveColor = aliveColor;
			this->deadColor = deadColor;
			this->emptyColor = emptyColor;
		}

		~golSettings_t() {};
	};

	struct cells_t
	{
		std::vector<cellState_t>		cells;

		cells_t()
		{

		}

		cells_t(golScene* gol, float dimensions)
		{
			std::srand(gol->randomSeed);
			for (size_t cellIter = 0; cellIter < (size_t)dimensions * dimensions; cellIter++)
			{
				size_t newRand = std::rand() % 100;
				cells.push_back((cellState_t)(newRand < gol->cellProbability));
			}
		}

		~cells_t()
		{

		}
	};

	golScene(float dimensions = 25.0f, GLdouble tickDelay = 0.3f, GLuint randomSeed = 666,
		GLuint cellProbability = 90, const char* windowName = "Ziyad Barakat's portfolio (game of life)",
		camera* golCamera = new camera(), const char* shaderConfigPath = "../../resources/shaders/GOL.txt")
		: scene(windowName, golCamera, shaderConfigPath)
	{
		this->tickDelay = tickDelay;
		this->randomSeed = randomSeed;
		this->cellProbability = cellProbability;

		//gol = bufferHandler_t<golSettings_t>(dimensions);
		gol.data.dimensions = dimensions;
		cellBuffer = bufferHandler_t<cells_t>(cells_t(this, dimensions));
		cellDimensions = glm::vec2(0);
		currentTickDelay = 0.0f;
	}

	~golScene(void) {};

protected:

	bufferHandler_t<golSettings_t>		gol;
	bufferHandler_t<cells_t>			cellBuffer;
	GLdouble							tickDelay;
	GLdouble							currentTickDelay;
	GLuint								randomSeed;
	GLuint								cellProbability;
	glm::vec2							cellDimensions;

	void SetupVertexBuffer() override
	{
		cellDimensions = glm::vec2(defaultPayload.data.resolution.x / gol.data.dimensions, defaultPayload.data.resolution.y / gol.data.dimensions);

		defaultVertexBuffer = new vertexBuffer_t(cellDimensions);
	}

	void Update() override
	{
		scene::Update();
		gol.Update();
		cellBuffer.Update(gl_shader_storage_buffer, gl_dynamic_draw, sizeof(int) * cellBuffer.data.cells.size(), cellBuffer.data.cells.data());
		//UpdateBuffer(gol, gol->bufferHandle, sizeof(*gol), gl_uniform_buffer, gl_dynamic_draw);
		//UpdateBuffer(cellBuffer->cells.data(), cellBuffer->bufferHandle, sizeof(int) * cellBuffer->cells.size(), gl_shader_storage_buffer, gl_dynamic_draw);
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
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, gol.data.dimensions * gol.data.dimensions);
		
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
		for (unsigned int cellIndex = 0; cellIndex < gol.data.dimensions * gol.data.dimensions; cellIndex++)
		{
			unsigned int column = cellIndex % (unsigned int)gol.data.dimensions;
			unsigned int row = cellIndex / (unsigned int)gol.data.dimensions;
			if (cellBuffer.data.cells[cellIndex] == EMPTY)
			{
				continue;
			}

			unsigned int neighborCount = 0;
			unsigned int deadNeighborCount = 0;
			//for convenience
			unsigned int dimensionMinOne = (unsigned int)gol.data.dimensions - 1;

			if (cellIndex < gol.data.dimensions)
			{
				//check if the Item is not in the last column
				if (cellIndex % (unsigned int)gol.data.dimensions == 0)
				{
					//next row, next column
					CheckNode(cellBuffer.data.cells[cellIndex + dimensionMinOne + 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer.data.cells[cellIndex + dimensionMinOne], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer.data.cells[cellIndex + 1], neighborCount, deadNeighborCount);
				}

				//check if its the last column in the row
				else if (cellIndex % dimensionMinOne == 0)
				{
					//next row, last column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer.data.cells[cellIndex + dimensionMinOne], neighborCount, deadNeighborCount);
					//this row, last column
					CheckNode(cellBuffer.data.cells[cellIndex - 1], neighborCount, deadNeighborCount);
				}

				else
				{
					//5 neighbors to consider
					//this row, last column
					CheckNode(cellBuffer.data.cells[cellIndex - 1], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer.data.cells[cellIndex + 1], neighborCount, deadNeighborCount);
					//next row, last column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne)], neighborCount, deadNeighborCount);
					//next row, next column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne) + 1], neighborCount, deadNeighborCount);
				}
			}

			//check if node is in the last row
			else if (cellIndex >
				(((gol.data.dimensions * gol.data.dimensions) - gol.data.dimensions) - 1)
				&& cellIndex < ((gol.data.dimensions * gol.data.dimensions) - 1))
			{
				//check if the column is a multiple of the dimension (the first column)
				if ((cellIndex % (unsigned int)gol.data.dimensions) == 0)
				{
					//last row, next column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne) + 1], neighborCount, deadNeighborCount); //lower middle node
																									//last row, this column
					CheckNode(cellBuffer.data.cells[cellIndex - dimensionMinOne], neighborCount, deadNeighborCount); //lower middle node
																							  //this row, next column
					CheckNode(cellBuffer.data.cells[cellIndex + 1], neighborCount, deadNeighborCount); //lower middle node
				}

				else if ((cellIndex % dimensionMinOne == 0))
				{
					//last row, this column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne)], neighborCount, deadNeighborCount); //lower middle node
																								//last row, last column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne) - 1], neighborCount, deadNeighborCount); //lower middle node
																									//this row, last column
					CheckNode(cellBuffer.data.cells[cellIndex - 1], neighborCount, deadNeighborCount); //lower middle node
				}

				else
				{
					//5 neighbors to consider
					//this row, last column
					CheckNode(cellBuffer.data.cells[cellIndex - 1], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer.data.cells[cellIndex + 1], neighborCount, deadNeighborCount);
					//last row, last column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//last row, this column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne)], neighborCount, deadNeighborCount);
					//last row, next column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne) + 1], neighborCount, deadNeighborCount);
				}
			}

			else
			{
				if (cellIndex < (gol.data.dimensions * gol.data.dimensions) - 1)
				{
					//8 neighbors to check
					//last row, last column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//last row, this column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne)], neighborCount, deadNeighborCount);
					//last row, next column
					CheckNode(cellBuffer.data.cells[(cellIndex - dimensionMinOne) + 1], neighborCount, deadNeighborCount);
					//this row, last column
					CheckNode(cellBuffer.data.cells[cellIndex - 1], neighborCount, deadNeighborCount);
					//this row, next column
					CheckNode(cellBuffer.data.cells[cellIndex + 1], neighborCount, deadNeighborCount);
					//next row, last column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne) - 1], neighborCount, deadNeighborCount);
					//next row, this column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne)], neighborCount, deadNeighborCount);
					//next row, next column
					CheckNode(cellBuffer.data.cells[(cellIndex + dimensionMinOne) + 1], neighborCount, deadNeighborCount);
				}
			}

			if (neighborCount < 2 && cellBuffer.data.cells[cellIndex] == ALIVE)
			{
				cellBuffer.data.cells[cellIndex] = DEAD;
			}

			else if (neighborCount >= 2 && cellBuffer.data.cells[cellIndex] == ALIVE)
			{
				if (neighborCount > 3)
				{
					cellBuffer.data.cells[cellIndex] = DEAD;
				}

				else
				{
					cellBuffer.data.cells[cellIndex] = ALIVE;
				}
			}

			else if (neighborCount == 3 && cellBuffer.data.cells[cellIndex] == DEAD)
			{
				cellBuffer.data.cells[cellIndex] = ALIVE;
			}
		}
	}

	void InitializeUniforms() override 
	{
		scene::InitializeUniforms();
		gol.Initialize(1);
		cellBuffer.Initialize(0, gl_shader_storage_buffer);
		//SetupBuffer(gol, gol->bufferHandle, sizeof(*gol), 1, gl_uniform_buffer, gl_dynamic_draw);
		//SetupBuffer(cellBuffer, cellBuffer->bufferHandle, GLuint(sizeof(int) * cellBuffer->cells.size()), 0, gl_shader_storage_buffer, gl_dynamic_draw);
	}

	virtual void Resize(tWindow* window, glm::ivec2 dimensions = glm::ivec2(0)) override
	{
		glViewport(0, 0, window->settings.resolution.x, window->settings.resolution.y);
		defaultPayload.data.resolution = glm::ivec2(window->settings.resolution.x, window->settings.resolution.y);
		defaultPayload.data.projection = glm::ortho(0.0f, (GLfloat)window->settings.resolution.x, (GLfloat)window->settings.resolution.y, 0.0f, 0.01f, 10.0f);
		cellDimensions = glm::ivec2(defaultPayload.data.resolution.x / gol.data.dimensions, defaultPayload.data.resolution.y / gol.data.dimensions);

		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
		//UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
		if (dimensions == glm::ivec2(0))
		{
			defaultVertexBuffer->UpdateBuffer(defaultPayload.data.resolution);
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
