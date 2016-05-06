#ifndef CAMERA_H
#define CAMERA_H


class camera
{
public:

	enum projectionType
	{
		PROJECTION_PERSPECTIVE = 0,
		PROJECTION_ORTHOGRAPHIC,
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
	projectionType		currentProjectionType;	

	camera( glm::vec2 resolution = glm::vec2( 1280, 720 ), float speed = 1.0f, 
			projectionType type = PROJECTION_ORTHOGRAPHIC, float nearPlane = 0.1f, 
			float farPlane = 100.0f, float fieldOfView = 45.0f )
	{
		this->farPlane = farPlane;
		this->nearPlane = nearPlane;
		this->fieldOfView = fieldOfView;
		this->speed = speed;
		this->currentProjectionType = type;
		this->translation = glm::mat4( 1.0f );
		this->resolution = resolution;
			
		(this->currentProjectionType == PROJECTION_ORTHOGRAPHIC) ? this->projection = glm::ortho( 0.0f , this->resolution.x, this->resolution.y, 
			0.0f, this->nearPlane, this->farPlane ) : 
			this->projection = glm::perspective( this->fieldOfView, this->resolution.x / this->resolution.y, 
					this->nearPlane, this->farPlane );

		//this->currentProjectionType ? this->view = glm::mat4(1) : glm::inverse(this->translation);

		if (currentProjectionType == PROJECTION_ORTHOGRAPHIC)
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
		if( currentProjectionType == PROJECTION_PERSPECTIVE )
		{
			this->view = glm::inverse(this->translation);
		}
	}


};

#endif
