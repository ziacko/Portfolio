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

	camera( glm::vec2 resolution = glm::vec2( 1280, 720 ), float speed = 1.0f, 
			projectionType type = PROJECTION_ORTHOGRAPHIC, float nearPlane = -1.0f, 
			float farPlane = 100.0f, float fieldOfView = 45.0f )
	{
		this->farPlane = farPlane;
		this->nearPlane = nearPlane;
		this->fieldOfView = fieldOfView;
		this->speed = speed;
		this->currentProjectionType = type;
		this->translation = glm::mat4( 1.0f );
		this->resolution = resolution;
			
		this->currentProjectionType ? this->projection = glm::ortho( 0.0f , this->resolution.x, 0.0f, 
			this->resolution.y, this->nearPlane, this->farPlane ) : 
			this->projection = glm::perspective( this->fieldOfView, this->resolution.x / this->resolution.y, 
					this->nearPlane, this->farPlane );

		this->view = glm::inverse(this->translation);
	}

	~camera( void ){}

	void Update( void )
	{
		if( currentProjectionType == PROJECTION_PERSPECTIVE )
		{
			this->view = glm::inverse(this->translation);
		}
	}

	glm::mat4 translation;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec2 mousePosition;
	glm::vec2 resolution;

	float speed;
	float fieldOfView;
	float nearPlane;
	float farPlane;

	projectionType currentProjectionType;	
};

#endif
