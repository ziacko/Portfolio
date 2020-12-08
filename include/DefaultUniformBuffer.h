#ifndef DEFAULT_UNIFORM_BUFFER_H
#define DEFAULT_UNIFORM_BUFFER_H
#include "UniformBuffer.h"
#include <array>

//change this into a payload system using templates
template<typename bufferType>
class bufferHandler_t
{
public:

	bufferHandler_t()
	{
		data = bufferType();
		bufferHandle = 0;
		uniformHandle = 0;
	}

	bufferHandler_t(bufferType payload)
	{
		this->data = payload;
		bufferHandle = 0;
		uniformHandle = 0;
	}

	//ok now we need functions to throw into this
	void Initialize(GLuint uniformHandle, GLenum target = gl_uniform_buffer, GLenum usage = gl_dynamic_draw)
	{
		this->uniformHandle = uniformHandle;
		glGenBuffers(1, &bufferHandle);
		Update(target, usage);
		glBindBufferBase(target, uniformHandle, bufferHandle);
	}

	void SetupUniforms(GLuint programGLID, std::string name, GLuint blockBindingIndex)
	{
		uniformHandle = glGetUniformBlockIndex(programGLID, name.c_str());
		glUniformBlockBinding(programGLID, uniformHandle, blockBindingIndex);
	}

	void Update(GLenum target = gl_uniform_buffer, GLenum usage = gl_dynamic_draw, size_t dataSize = 0, void* inData = nullptr)
	{
		glBindBuffer(target, bufferHandle);
		if(dataSize > 0 && inData != nullptr)
		{
			glBufferData(target, dataSize, inData, usage);
		}
		else
		{
			glBufferData(target, sizeof(data), &data, usage);
		}
		
		//printf("%i \n", sizeof(data));
	}

	void Override(unsigned int uniformHandle, GLenum target = gl_uniform_buffer, GLenum usage = gl_dynamic_draw, size_t dataSize = 0, void* inData = nullptr)
	{
		//ok so this is for overriding the data in existing shader storage buffers
		//might have to look for a better system later
		//glBindBuffer(target, bufferHandle); //i don't think re-setting the bufferhandle is needed here
		glBindBufferBase(target, uniformHandle, bufferHandle);
		if (dataSize > 0 && inData != nullptr)
		{
			glBufferData(target, dataSize, inData, usage);
			glFinish();
		}
	}

	void BindToSlot(unsigned int uniformHandle, GLenum target = gl_uniform_buffer)
	{
		//glBindBuffer(target, bufferHandle);
		glBindBufferBase(target, uniformHandle, bufferHandle);
		//Update(target, gl_dynamic_draw);
		this->uniformHandle = uniformHandle;
	}

	bufferType data;
	unsigned int bufferHandle;
	unsigned int uniformHandle;
};

class defaultUniformBuffer// : public uniformBuffer_t
{
public:

	glm::mat4			projection;
	glm::mat4			view;
	glm::mat4			translation;
	glm::vec2			resolution;
	glm::vec2			mousePosition;
	GLfloat				deltaTime;
	GLfloat				totalTime;
	GLfloat				framesPerSec;
	GLuint				totalFrames;

	defaultUniformBuffer( glm::mat4 projection, glm::mat4 view,
			glm::mat4 translation = glm::mat4( 1 ), glm::ivec2 resolution = glm::ivec2(1280, 720) )
		//: uniformBuffer_t()
	{
		//BuildBuffer();
		//uniformBuffer_t();
		this->projection = projection;
		this->view = view;
		this->translation = translation;
		this->resolution = resolution;
		totalFrames = 1;
	}

	defaultUniformBuffer(camera* defaultCamera)// : uniformBuffer_t()
	{
		//uniformBuffer_t();
		//BuildBuffer();
		this->projection = defaultCamera->projection;
		this->view = defaultCamera->view;
		this->translation = defaultCamera->translation;
		this->resolution = defaultCamera->resolution;
		totalFrames = 1;
	}

	defaultUniformBuffer(){};

/*
	virtual void* GetBuffer() override
	{
		return data;
	}

	virtual void BuildBuffer() override
	{
		AppendBuffer<glm::mat4>(projection, data);
		AppendBuffer<glm::mat4>(view, data);
		AppendBuffer<glm::mat4>(translation, data);
		AppendBuffer<glm::vec2>(resolution, data);
		AppendBuffer<glm::vec2>(mousePosition, data);
		AppendBuffer<float>(deltaTime, data);
		AppendBuffer<float>(totalTime, data);
		AppendBuffer<float>(framesPerSec, data);
		printf("%i \n", dataSize);
	}*/
};
#endif
