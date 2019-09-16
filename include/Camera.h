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
	const float			defaultOrthoNear = 0.01f;
	const float			defaultOrthoFar = 100.0f;
	const float			defaultPersNear = 15.0f;
	const float			defaultPersFar = 1000.0f;
	float				farPlane;
	float				nearPlane;
	projection_t		currentProjectionType;

	float				xSensitivity;
	float				ySensitivity;
	float				zSensitivity;

	float				yaw = 0.0f;
	float				pitch = 0.0f;
	float				roll = 0.0f;

	//glm::vec4 rotation;
	glm::vec3 position;
	glm::vec3 rotator; //roll, pitch, yaw
	glm::quat rotation;

	bool guiActive = false;

	camera(glm::vec2 resolution = glm::vec2(1280, 720), float speed = 1.0f,
		projection_t type = projection_t::orthographic, float nearPlane = 0.1f,
		float farPlane = 1000.0f, float fieldOfView = 60.0f)
	{
		this->farPlane = farPlane;
		this->nearPlane = nearPlane;
		this->fieldOfView = fieldOfView;
		this->speed = speed;
		this->currentProjectionType = type;
		this->translation = glm::mat4(1.0f);
		this->resolution = resolution;
		xSensitivity = 0.01f;
		ySensitivity = 0.01f;
		zSensitivity = 0.01f;

		(this->currentProjectionType == projection_t::orthographic) ? this->projection = glm::ortho(0.0f, this->resolution.x, this->resolution.y,
			0.0f, this->nearPlane, this->farPlane) :
			this->projection = glm::perspective<float>(this->fieldOfView, this->resolution.x / this->resolution.y,
				this->nearPlane, this->farPlane);

		if (currentProjectionType == projection_t::orthographic)
		{
			this->view = glm::mat4(1);
			this->view[3][2] = -5.0f;
		}

		else
		{
			this->view = glm::inverse(this->translation);
		}

		position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		rotation = glm::quat(glm::vec3(0.0f));
		rotator = glm::vec3(0.0f);
	}

	~camera() {}

	void Update()
	{
		if (currentProjectionType == projection_t::perspective)
		{
			view = glm::eulerAngleXYZ(rotator.y, rotator.x, rotator.z);
			view = glm::translate(view, -position);
			rotation = glm::toQuat(view);
		}
		UpdateProjection();
	}

	void UpdateProjection()
	{
		if (currentProjectionType == projection_t::perspective)
		{
			if (resolution.x > 0 && resolution.y > 0)
			{
				projection = glm::perspective<float>(glm::radians(fieldOfView), resolution.x / resolution.y,
					nearPlane, farPlane);
			}
		}

		else
		{
			projection = glm::ortho<float>(0.0f, resolution.x, resolution.y, 0.0f, defaultOrthoNear, defaultOrthoFar);
		}
	}

	void ChangeProjection(projection_t newProjection)
	{
		currentProjectionType = newProjection;

		if (currentProjectionType == projection_t::perspective)
		{
			if (resolution.x > 0 && resolution.y > 0)
			{
				projection = glm::perspective<float>(glm::radians(fieldOfView), resolution.x / resolution.y,
					nearPlane, farPlane);
				view = glm::eulerAngleXYZ(rotator.y, rotator.x, rotator.z);
				view = glm::translate(view, -position);
				rotation = glm::toQuat(view);
			}
		}

		else
		{
			//nearPlane = defaultOrthoNear;
			//farPlane = defaultOrthoFar;
			projection = glm::ortho<float>(0.0f, resolution.x, resolution.y, 0.0, defaultOrthoNear, defaultOrthoFar);
			this->view = glm::mat4(1);
			this->view[3][2] = -5.0f;
			this->translation = glm::mat4(1.0f);
		}
	}

	void Pitch(float pitchRadians)
	{
		rotator.y += pitchRadians;
	}

	void Yaw(float yawRadians)
	{
		rotator.z += yawRadians;
	}

	void Roll(float rollRadians)
	{
		rotator.x += rollRadians;
	}

	glm::vec3 GetForward()
	{
		return glm::conjugate(rotation) * glm::vec3(0.0f, 0.0f, -1.0f);
	}

	glm::vec3 GetRight()
	{
		return glm::conjugate(rotation) * glm::vec3(1.0f, 0.0f, 0.0f);
	}

	glm::vec3 GetUp()
	{
		return glm::conjugate(rotation) * glm::vec3(0.0f, 1.0f, 0.0f);
	}

	void MoveForward(float movement, float deltaTime)
	{
		position += (GetForward() * movement) * (1 - deltaTime);
	}

	void MoveRight(float movement, float deltaTime)
	{
		position += (GetRight() * movement) * (1 - deltaTime);
	}

	void MoveUp(float movement, float deltaTime)
	{
		position += (GetUp() * movement) * (1 - deltaTime);
	}

	//rebindable actions
	class command
	{
	public:

		virtual ~command() {};
		virtual void Execute(camera* cam, float speed, float deltaTime) = 0;
	};

	class up : public command
	{
	public:

		virtual void Execute(camera* cam, float speed, float deltaTime) override
		{
			cam->MoveUp(-speed, deltaTime);
		}
	};

	class down : public command
	{
	public:

		virtual void Execute(camera* cam, float speed, float deltaTime) override
		{
			cam->MoveUp(speed, deltaTime);
		}
	};

	class left : public command
	{
	public:

		virtual void Execute(camera* cam, float speed, float deltaTime) override
		{
			cam->MoveRight(-speed, deltaTime);
		}
	};

	class right : public command
	{
	public:

		virtual void Execute(camera* cam, float speed, float deltaTime) override
		{
			cam->MoveRight(speed, deltaTime);
		}
	};

	class forwards : public command
	{
	public:

		virtual void Execute(camera* cam, float speed, float deltaTime) override
		{
			cam->MoveForward(speed, deltaTime);
		}
	};

	class backwards : public command
	{
	public:

		virtual void Execute(camera* cam, float speed, float deltaTime) override
		{
			cam->MoveForward(-speed, deltaTime);
		}
	};
};

#endif
