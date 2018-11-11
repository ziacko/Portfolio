#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

class uniformBuffer_t
{
public:
	GLuint bufferHandle;
	GLuint uniformHandle;

	GLuint dataSize;
	void* data;

	uniformBuffer_t()
	{
		dataSize = 0;
		bufferHandle = NULL;
		uniformHandle = NULL;
		data = CreateBaseBuffer();
		//BuildBuffer();
	}

	void Update(void* data, GLuint bufferHandle, GLintptr offset, GLuint bufferSize, GLenum target, GLenum usage)
	{
		glBindBuffer(target, bufferHandle);
		glBufferSubData(target, offset, bufferSize, data);
	}

	void Setup(void* data, GLuint& bufferHandle, GLintptr offset, GLuint bufferSize, GLuint uniformHandle, GLenum target, GLenum usage)
	{
		glGenBuffers(1, &bufferHandle);
		Update(data, bufferHandle, offset, bufferSize, target, usage);
		glBindBufferBase(target, uniformHandle, bufferHandle);
	}

	void* CreateBaseBuffer()
	{
		return (void*)malloc(sizeof(*this) - (sizeof(GLuint) * 2));
	}

	virtual void* GetBuffer() = 0;

	virtual void BuildBuffer() = 0;

	template<typename t>
	void AppendBuffer(t object, void*& buffer)
	{
		memcpy(buffer, &object, sizeof(object));
		buffer = (void*)(((char*)buffer) + sizeof(object));
		dataSize += sizeof(object);
	}
};

#endif