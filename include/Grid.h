#pragma once

class grid
{

public:

	unsigned int vertexArrayHandle;
	unsigned int vertexBufferHandle;
	unsigned int indexBufferHandle;
	glm::ivec2 dimensions;	
	glm::mat4 position;
	glm::mat4 scale;
	glm::mat4 rotation;
	std::vector<glm::vec4> vertices;
	std::vector<unsigned int> indices;

	grid(glm::ivec2 dimensions)
	{
		this->dimensions = dimensions;
		vertices = std::vector<glm::vec4>(dimensions.x * dimensions.y);
		indices = std::vector<unsigned int>((dimensions.x * dimensions.y) * 6);
		vertexArrayHandle = 0;
		vertexBufferHandle = 0;
		indexBufferHandle = 0;

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
				vertices[rowIter * dimensions.y + columnIter] = glm::vec4(columnIter, 1.0f, rowIter, 1.0f);
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

		glBufferData(gl_array_buffer, vertices.size() * sizeof(glm::vec4), vertices.data(), gl_static_draw);
		glBufferData(gl_element_array_buffer, indices.size() * sizeof(unsigned int), &indices[0], gl_static_draw);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (char*)0);

		glBindVertexArray(0);
	}

	void Draw()
	{
		glBindBuffer(gl_element_array_buffer, indexBufferHandle);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}
};