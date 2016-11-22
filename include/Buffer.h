#ifndef BUFFER_H
#define BUFFER_H

class buffer
{
public:

	buffer()
	{
		glGenBuffers(1, &handle);
	}

	virtual void Update(GLsizeiptr size, void* data, GLenum usage)
	{
		glBindBuffer(target, handle);
		glBufferData(target, size, data, usage);
	}

protected:
	GLuint handle;
	GLenum target;
	GLsizeiptr size;
	GLenum usage;
};
#endif // BUFFER_H
