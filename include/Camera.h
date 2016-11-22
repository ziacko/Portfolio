#ifndef CAMERA_H
#define CAMERA_H

class camera
{
public:

	enum class projection_t
	{
		perspective,
		orthographic,
	};

	glm::mat4			translation;
	glm::mat4			projection;
	glm::mat4			view;
	glm::vec2			mousePosition;
	glm::vec2			resolution;
	float				speed;
	float				fieldOfView;
	float				nearPlane;
	float				farPlane;
	projection_t		currentProjectionType;	

	float				xSensitivity;
	float				ySensitivity;

	camera( glm::vec2 resolution = glm::vec2( 1280, 720 ), float speed = 1.0f, 
			projection_t type = projection_t::orthographic, float nearPlane = 0.1f, 
			float farPlane = 1000.0f, float fieldOfView = 45.0f )
	{
		this->farPlane = farPlane;
		this->nearPlane = nearPlane;
		this->fieldOfView = fieldOfView;
		this->speed = speed;
		this->currentProjectionType = type;
		this->translation = glm::mat4();
		this->resolution = resolution;
		xSensitivity = 1.0f;
		ySensitivity = 1.0f;
			
		(this->currentProjectionType == projection_t::orthographic) ? this->projection = glm::ortho( 0.0f , this->resolution.x, this->resolution.y,
			0.0f, this->nearPlane, this->farPlane ) : 
			this->projection = glm::perspective<float>( this->fieldOfView, this->resolution.x / this->resolution.y, 
					this->nearPlane, this->farPlane );

		//this->currentProjectionType ? this->view = glm::mat4(1) : glm::inverse(this->translation);

		if (currentProjectionType == projection_t::orthographic)
		{
			this->view = glm::mat4(1);
			this->view[3][2] = -5.0f;
		}

		else
		{
			//this->translation[3][2] = -5.0f;
			this->view = glm::inverse(this->translation);
		}
	}

	~camera( void ){}

	void Update( void )
	{
		if( currentProjectionType == projection_t::perspective )
		{
			this->view = glm::inverse(this->translation);
		}
	}

	void UpdateProjection()
	{
		if (currentProjectionType == projection_t::perspective)
		{
			projection = glm::perspective<float>(glm::radians(fieldOfView), resolution.x / resolution.y,
				nearPlane, farPlane);
		}

		else
		{
			projection = glm::ortho<float>(0.0f, resolution.x, resolution.y, 0.0f, nearPlane, farPlane);
		}
	}
};

#endif
