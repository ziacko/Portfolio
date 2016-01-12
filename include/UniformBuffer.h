#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

class uniformBuffer_t
{
public:
	GLuint bufferHandle;
	GLuint uniformHandle;

	uniformBuffer_t()
	{
		bufferHandle = NULL;
		uniformHandle = NULL;
	}
	//virtual ~uniformBuffer_t(){}

};

#endif