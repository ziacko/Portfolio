#ifndef LIGHT_H
#define LIGHT_H

typedef enum {directional = 0, point = 1, spot = 2} lightType_t;

class light //set this up as uniform buffer friendly?
{

public:

	glm::vec4					color;
	glm::quat					direction;

	//necessary for point and spot lights
	glm::vec4					position;

	//general settings
	float						intensity;

	//for spot light
	float						angle;

	//for point light
	float						range;

	int							currentType;

	light(lightType_t lightType = lightType_t::directional, glm::vec4 color = glm::vec4(1),
		glm::quat direction = glm::quat(glm::vec3(0)),	glm::vec4 position = glm::vec4(0), 
		float intensity = 1.0f, float angle = 1.0f, float range = 1.0f)
	{
		this->currentType = lightType;
		this->color = color;
		this->direction = direction;
		this->position = position;
		this->intensity = intensity;
		this->angle = angle;
		this->range = 0;

		bufferHandle = 0;
		uniformHandle = 0;
	}

	std::vector<const char*>	lightTypes = { "directional", "point", "spot" };

	unsigned int				bufferHandle;
	unsigned int				uniformHandle;
};

#endif