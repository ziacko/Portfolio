#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class vertexBuffer_t
{
public:

	GLuint		bufferHandle;
	GLuint		vertexArrayHandle;

	vertexBuffer_t()
	{

	}

	vertexBuffer_t(glm::vec2 resolution)
	{
		bufferHandle = 0;
		vertexArrayHandle = 0;
		GLfloat quadVerts[16] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			resolution.x, 0.0f, 1.0f, 1.0f,
			resolution.x, resolution.y, 1.0f, 1.0f,
			0.0f, resolution.y, 1.0f, 1.0f
		};

		glGenBuffers(1, &bufferHandle);
		glGenVertexArrays(1, &vertexArrayHandle);
		glBindVertexArray(bufferHandle);

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quadVerts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (char*)(sizeof(float) * 4));
	}

	void UpdateBuffer(glm::vec2 resolution)
	{
		GLfloat newVerts[16] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			resolution.x, 0.0f, 1.0f, 1.0f,
			resolution.x, resolution.y, 1.0f, 1.0f,
			0.0f, resolution.y, 1.0f, 1.0f
		};

		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, newVerts, GL_STATIC_DRAW);

	}

};


#endif