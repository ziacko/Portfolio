#ifndef COMMAND_H
#define COMMAND_H

#include <Scene3D.h>

class commandScene : public scene3D
{
public:

	commandScene(const char* windowName = "Ziyad Barakat's Portfolio(command)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 200.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/Model.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX")) :
		scene3D(windowName, camera3D, shaderConfigPath, model)
	{
		testModel = model;
		materialSettingsBuffer = new baseMaterialSettings_t();
		wireframe = false;
	}

	~commandScene() {};

	virtual void SetupCallbacks() override
	{
		manager->resizeEvent = std::bind(&commandScene::HandleWindowResize, this, _1, _2);
		manager->maximizedEvent = std::bind(&commandScene::HandleMaximize, this, _1);
		//manager->destroyedEvent = std::bind(&command::ShutDown, this, _1);

		manager->mouseButtonEvent = std::bind(&commandScene::HandleMouseClick, this, _1, _2, _3);
		manager->mouseMoveEvent = std::bind(&commandScene::HandleMouseMotion, this, _1, _2, _3);
		manager->keyEvent = std::bind(&commandScene::HandleKey, this, _1, _2, _3);
	}

	virtual void Initialize() override
	{
		scene3D::Initialize();
	}

protected:
	
	camera::up cameraUpProc;
	camera::down cameraDownProc;
	camera::left cameraLeftProc;
	camera::right cameraRightProc;
	camera::forwards cameraForwardsProc;
	camera::backwards cameraBackwardsProc;

	virtual void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state) override
	{
		scene::HandleMouseClick(window, button, state);
	}

	virtual void HandleMouseMotion(tWindow* window, vec2_t<int> windowPosition, vec2_t<int> screenPosition) override
	{
		//commandScene* thisScene = (commandScene*)window->userData;
		scene::HandleMouseMotion(window, windowPosition, screenPosition);

		glm::vec2 mouseDelta = glm::vec2(window->mousePosition.x - window->previousMousePosition.x, window->mousePosition.y - window->previousMousePosition.y);
		float deltaTime = (float)sceneClock->GetDeltaTime();
		if (window->mouseButton[(int)mouseButton_t::right] == buttonState_t::down)
		{
			if (mouseDelta.x != 0)
			{
				sceneCamera->Yaw((float)((mouseDelta.x * sceneCamera->xSensitivity) * (1 - deltaTime)));
			}

			if (mouseDelta.y != 0)
			{
				sceneCamera->Pitch((float)((mouseDelta.y * sceneCamera->ySensitivity) * (1 - deltaTime)));
			}
		}
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		//commandScene* thisScene = (commandScene*)window->userData;
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		sceneCamera->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		defaultUniform->resolution = sceneCamera->resolution;
		sceneCamera->UpdateProjection();
		defaultUniform->projection = sceneCamera->projection;

		//bind the uniform buffer and refill it
		UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		//commandScene* thisScene = (commandScene*)window->userData;

		glViewport(0, 0, dimensions.width, dimensions.height);
		sceneCamera->resolution = glm::vec2(dimensions.width, dimensions.height);
		defaultUniform->resolution = sceneCamera->resolution;
		sceneCamera->UpdateProjection();
		defaultUniform->projection = sceneCamera->projection;
		defaultUniform->deltaTime = (float)sceneClock->GetDeltaTime();
		defaultUniform->totalTime = (float)sceneClock->GetTotalTime();
		defaultUniform->framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());

		UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void HandleKey(tWindow* window, int key, keyState_t state)	override
	{
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
			camSpeed = sceneCamera->speed * 2;
		}

		else
		{
			camSpeed = sceneCamera->speed;
		}

		float deltaTime = (float)sceneClock->GetDeltaTime();

		if (state == keyState_t::down) //instead of one key could we check multiple keys?
		{
			if (window->keys['w'] == keyState_t::down)
			{
				cameraForwardsProc.Execute(sceneCamera, camSpeed, deltaTime);
			}

			if (window->keys['s'] == keyState_t::down)
			{
				cameraBackwardsProc.Execute(sceneCamera, camSpeed, deltaTime);
			}

			if (window->keys['a'] == keyState_t::down)
			{
				cameraLeftProc.Execute(sceneCamera, camSpeed, deltaTime);
			}

			if (window->keys['d'] == keyState_t::down)
			{
				cameraRightProc.Execute(sceneCamera, camSpeed, deltaTime);
			}

			if (window->keys['e'] == keyState_t::down)
			{				
				cameraDownProc.Execute(sceneCamera, camSpeed, deltaTime);
			}

			if (window->keys['q'] == keyState_t::down)
			{
				cameraUpProc.Execute(sceneCamera, camSpeed, deltaTime);				
			}

			if (window->keys['z'] == keyState_t::down)
			{
				sceneCamera->Roll(glm::radians((float)sceneCamera->zSensitivity * deltaTime));
			}

			if (window->keys['x'] == keyState_t::down)
			{
				sceneCamera->Roll(glm::radians((float)-sceneCamera->zSensitivity * deltaTime));
			}
		}
	}
};

#endif