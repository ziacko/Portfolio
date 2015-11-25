#ifndef DEFAULT_UNIFORM_BUFFER_H
#define DEFAULT_UNIFORM_BUFFER_H

struct defaultUniformBuffer_t
{
	glm::mat4 projection;
	glm::mat4 view;
	glm::mat4 translation;

	//use GLfloat so they fit perfectly in buffer!
	GLfloat width;
	GLfloat height;

	defaultUniformBuffer_t( glm::mat4 projection, glm::mat4 view,
			glm::mat4 translation = glm::mat4( 1 ), glm::vec2 resolution = glm::vec2(1280, 720) )
	{
		this->projection = projection;
		this->view = view;
		this->translation = translation;
		this->width = resolution.x;
		this->height = resolution.y;
	}

	defaultUniformBuffer_t(camera* defaultCamera)
	{
		this->projection = defaultCamera->projection;
		this->view = defaultCamera->view;
		this->translation = defaultCamera->translation;
		this->width = defaultCamera->resolution.x;
		this->height = defaultCamera->resolution.y;

	}

	defaultUniformBuffer_t(){};
};
#endif
