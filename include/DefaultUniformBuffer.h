#ifndef DEFAULT_UNIFORM_BUFFER_H
#define DEFAULT_UNIFORM_BUFFER_H
#include "UniformBuffer.h"

class defaultUniformBuffer_t// : public uniformBuffer_t
{
public:

	glm::mat4			projection;
	glm::mat4			view;
	glm::mat4			translation;
	glm::vec2			resolution;
	glm::vec2			mousePosition;
	GLdouble			deltaTime;
	GLdouble			totalTime;
	GLdouble			framesPerSec;
	GLuint				bufferHandle;
	GLuint				uniformHandle;

	defaultUniformBuffer_t( glm::mat4 projection, glm::mat4 view,
			glm::mat4 translation = glm::mat4( 1 ), glm::vec2 resolution = glm::vec2(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) )
	{
		//uniformBuffer_t();
		this->projection = projection;
		this->view = view;
		this->translation = translation;
		this->resolution = resolution;
	}

	defaultUniformBuffer_t(camera* defaultCamera)
	{
		//uniformBuffer_t();
		this->projection = defaultCamera->projection;
		this->view = defaultCamera->view;
		this->translation = defaultCamera->translation;
		this->resolution = defaultCamera->resolution;
	}

	defaultUniformBuffer_t(){};
};
#endif
