#pragma once

#include "VertexAttribute.h"

class grid
{

public:

	unsigned int vertexArrayHandle;
	unsigned int vertexBufferHandle;
	unsigned int indexBufferHandle;
	glm::ivec2 dimensions;
	std::vector<vertexAttribute_t> vertices;
	std::vector<unsigned int> indices;
	bool tiled;

	grid(glm::ivec2 dimensions, bool tiled = false)
	{
		this->dimensions = dimensions;
		vertices = std::vector<vertexAttribute_t>(dimensions.x * dimensions.y);
		indices = std::vector<unsigned int>((dimensions.x * dimensions.y) * 6);
		vertexArrayHandle = 0;
		vertexBufferHandle = 0;
		indexBufferHandle = 0;
		this->tiled = tiled;

		//create the grid from here and store it a as a vector of vec4?
		GenerateGrid();
		GenerateBuffers();
	}

	void GenerateGrid()
	{
		int index = 0;
		for (int rowIter = 0; rowIter < dimensions.x; ++rowIter)
		{
			for (int columnIter = 0; columnIter < dimensions.y; ++columnIter)
			{
				vertices[rowIter * dimensions.y + columnIter].position = glm::vec4(columnIter, rowIter, 1.0f, 1.0f);
				vertices[rowIter * dimensions.y + columnIter].normal = glm::vec4(0, 1, 0, 0);
				if(tiled)
				{
					vertices[rowIter * dimensions.y + columnIter].uv = glm::vec2(columnIter, rowIter);
				}

				else
				{
					vertices[rowIter * dimensions.y + columnIter].uv = glm::vec2(columnIter / (float)(dimensions.y - 1), rowIter / (float)(dimensions.x - 1));
				}
				
			}
		}

		for (int rowIter = 0; rowIter < dimensions.x - 1; rowIter++)
		{
			for (int columnIter = 0; columnIter < dimensions.y - 1; columnIter++)
			{
				//triangle 1
				indices[index++] = ((rowIter + 1) * dimensions.y + (columnIter + 1));
				indices[index++] = ((rowIter + 1) * dimensions.y + columnIter);
				indices[index++] = (rowIter * dimensions.y + columnIter);

				//triangle 2
				indices[index++] = (rowIter * dimensions.y + (columnIter +1));
				indices[index++] = ((rowIter + 1) * dimensions.y + (columnIter + 1));
				indices[index++] = (rowIter * dimensions.y + columnIter);
			}
		}
	}

	void GenerateBuffers()
	{
		glGenBuffers(1, &vertexBufferHandle);
		glGenBuffers(1, &indexBufferHandle);
		glGenVertexArrays(1, &vertexArrayHandle);

		glBindVertexArray(vertexArrayHandle);
		glBindBuffer(gl_array_buffer, vertexBufferHandle);
		glBindBuffer(gl_element_array_buffer, indexBufferHandle);

		glBufferData(gl_array_buffer, vertices.size() * sizeof(vertexAttribute_t), vertices.data(), gl_static_draw);
		glBufferData(gl_element_array_buffer, indices.size() * sizeof(unsigned int), &indices[0], gl_static_draw);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::position);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::normal);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::tangent);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::biNormal);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(vertexAttribute_t), (char*)vertexOffset::uv);
		glBindVertexArray(0);
	}

	void Draw()
	{
		glBindBuffer(gl_element_array_buffer, indexBufferHandle);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
};