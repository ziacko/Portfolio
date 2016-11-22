#ifndef SCENE3D_H
#define SCENE3D_H
#include "Scene.h"
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include "Grid.h"
#include "FrameBuffer.h"

class scene3D : public scene
{
public:

	scene3D(const char* windowName = "Ziyad Barakat's Portfolio(3D scene)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 100.0f, camera::projection_t::perspective),
		const char* shaderConfigPath = "../../resources/shaders/Model.txt") : 
		scene(windowName, camera3D, shaderConfigPath)
	{
		
	}

	~scene3D() {};

	//override input code. use this to mess with camera
	virtual void SetupCallbacks() override 
	{
		manager->resizeEvent = &scene3D::HandleWindowResize;
		manager->maximizedEvent = &scene3D::HandleMaximize;
		manager->destroyedEvent = &scene::ShutDown;

		manager->mouseButtonEvent = &scene3D::HandleMouseClick;
		manager->mouseMoveEvent = &scene3D::HandleMouseMotion;
		manager->keyEvent = &scene3D::HandleKeyPress;
	}

	virtual void Initialize() override
	{
		scene::Initialize();
		drawGrid = new grid(glm::ivec2(100, 100));
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
	}

protected:

	grid* drawGrid;

	virtual void Draw() override 
	{
		glBindVertexArray(drawGrid->vertexArrayHandle);
		glUseProgram(this->programGLID);
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		drawGrid->Draw();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		DrawGUI(window->name);

		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	virtual void BuildGUI(ImGuiIO io) override
	{
		scene::BuildGUI(io);
		ImGui::InputFloat("camera speed", &sceneCamera->speed, 0.f);
		ImGui::InputFloat("x sensitivity", &sceneCamera->xSensitivity, 0.f);
		ImGui::InputFloat("y sensitivity", &sceneCamera->ySensitivity, 0.f);

		if (ImGui::SliderFloat("Field of view", &sceneCamera->fieldOfView, 0, 180, "%.10f"))
		{
			sceneCamera->UpdateProjection();
			defaultUniform->projection = sceneCamera->projection;
			UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		}
	}

	virtual void InitializeBuffers() override
	{
		defaultUniform = new defaultUniformBuffer(sceneCamera);
		glViewport(0, 0, window->resolution.width, window->resolution.height);

		defaultUniform->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		defaultUniform->projection = sceneCamera->projection;
		defaultUniform->translation = sceneCamera->translation;
		defaultUniform->view = sceneCamera->view;

		//SetupVertexBuffer();
		SetupBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), 0, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	static void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state)
	{
		scene::HandleMouseClick(window, button, state);
	}

	static void HandleMouseMotion(tWindow* window, vec2_t<int> windowPosition, vec2_t<int> screenPosition)
	{
		scene3D* thisScene = (scene3D*)window->userData;
		scene::HandleMouseMotion(window, windowPosition, screenPosition);
		//
		glm::vec4 right = thisScene->sceneCamera->translation[0];
		glm::vec4 up = thisScene->sceneCamera->translation[1];
		glm::vec4 forward = thisScene->sceneCamera->translation[2];
		glm::vec4 translation = thisScene->sceneCamera->translation[3];

		if (window->mouseButton[(int)mouseButton_t::right] == buttonState_t::down)
		{
			//window->mousePosition
			glm::vec2 mouseDelta = glm::vec2(window->mousePosition.x - window->previousMousePosition.x, window->mousePosition.y - window->previousMousePosition.y);
			glm::mat4 matrix;
			if (mouseDelta.x != 0)
			{
				//rotate on the Y axis. do not be an inverted peasant!
				matrix = glm::rotate(matrix, (float)((mouseDelta.x * thisScene->sceneCamera->xSensitivity) * (float)thisScene->sceneClock->GetDeltaTime()), glm::vec3(up.x, up.y, up.z));
				forward = matrix * forward;
				up = matrix * up;
				right = matrix * right;
			}

			if (mouseDelta.y != 0)
			{
				//rotate on the X axis. 
				matrix = glm::rotate(matrix, (float)((mouseDelta.y * thisScene->sceneCamera->ySensitivity) * (float)thisScene->sceneClock->GetDeltaTime()), glm::vec3(right.x, right.y, right.z));
				forward = matrix * forward;
				up = matrix * up;
				right = matrix * right;
			}
			thisScene->sceneCamera->translation = glm::mat4(right, up, forward, translation);
			//window->SetMousePosition(TinyWindow::vec2_t<unsigned int>(thisScene->sceneCamera->resolution.x / 2, thisScene->sceneCamera->resolution.y / 2));
		}
		thisScene->defaultUniform->translation = thisScene->sceneCamera->translation;
		UpdateBuffer(thisScene->defaultUniform, thisScene->defaultUniform->bufferHandle, sizeof(defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	static void HandleMaximize(tWindow* window)
	{
		scene3D* thisScene = (scene3D*)window->userData;
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		thisScene->sceneCamera->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		thisScene->defaultUniform->resolution = thisScene->sceneCamera->resolution;
		thisScene->sceneCamera->UpdateProjection();
		thisScene->defaultUniform->projection = thisScene->sceneCamera->projection;

		//bind the uniform buffer and refill it
		UpdateBuffer(thisScene->defaultUniform, thisScene->defaultUniform->bufferHandle, sizeof(defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	static void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions)
	{
		scene3D* thisScene = (scene3D*)window->userData;

		glViewport(0, 0, dimensions.width, dimensions.height);
		thisScene->sceneCamera->resolution = glm::vec2(dimensions.width, dimensions.height);
		thisScene->defaultUniform->resolution = thisScene->sceneCamera->resolution;
		thisScene->sceneCamera->UpdateProjection();
		thisScene->defaultUniform->projection = thisScene->sceneCamera->projection;

		UpdateBuffer(thisScene->defaultUniform, thisScene->defaultUniform->bufferHandle, sizeof(defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	static void HandleKeyPress(tWindow* window, int key, keyState_t state)
	{
		scene3D* thisScene = (scene3D*)window->userData;
		glm::vec4 right = thisScene->sceneCamera->translation[0];
		glm::vec4 up = thisScene->sceneCamera->translation[1];
		glm::vec4 forward = thisScene->sceneCamera->translation[2];
		glm::vec4 translation = thisScene->sceneCamera->translation[3];

		ImGuiIO& io = ImGui::GetIO();
		if (state == keyState_t::down)
		{
			io.KeysDown[key] = true;
			io.AddInputCharacter(key);
		}

		else
		{
			io.KeysDown[key] = false;
		}
		
		float camSpeed = 0.0f;
		if (key == key_t::leftShift && state == keyState_t::down)
		{
			camSpeed = thisScene->sceneCamera->speed * 2;
		}
		
		else
		{
			camSpeed = thisScene->sceneCamera->speed;
		}

		if (state == keyState_t::down)
		{
			switch (key)
			{
			case 'W':
			{
				translation += forward * (camSpeed * (float)thisScene->sceneClock->GetDeltaTime());
				break;
			}

			case 'S':
			{
				translation -= forward * (camSpeed * (float)thisScene->sceneClock->GetDeltaTime());
				break;
			}

			case 'A':
			{
				translation += right * (camSpeed * (float)thisScene->sceneClock->GetDeltaTime());
				break;
			}

			case 'D':
			{
				translation -= right * (camSpeed * (float)thisScene->sceneClock->GetDeltaTime());
				break;
			}
			
			case 'E':
			{
				translation -= up * (camSpeed * (float)thisScene->sceneClock->GetDeltaTime());
				break;
			}

			case 'Q':
			{
				translation += up * (camSpeed * (float)thisScene->sceneClock->GetDeltaTime());
				break;
			}

			default:
			{
				break;
			}
			}

			thisScene->sceneCamera->translation[3] = translation;
		}
		thisScene->defaultUniform->translation = thisScene->sceneCamera->translation;
		UpdateBuffer(thisScene->defaultUniform, thisScene->defaultUniform->bufferHandle, sizeof(defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}
};

#endif

