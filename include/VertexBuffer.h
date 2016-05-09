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
		try
		{
			glBindVertexArray(bufferHandle);
		}

		catch (std::exception* e)
		{
			std::cout << e->what();
			//printf("%s \n", e->what());
			return;
		}


		//glBindVertexArray(bufferHandle);

		glBindBuffer(gl_array_buffer, bufferHandle);
		glBufferData(gl_array_buffer, sizeof(float) * 16, quadVerts, gl_static_draw);
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

		glBindBuffer(gl_array_buffer, bufferHandle);
		glBufferData(gl_array_buffer, sizeof(float) * 16, newVerts, gl_static_draw);

	}

};


#endif