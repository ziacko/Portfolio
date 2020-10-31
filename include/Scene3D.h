#ifndef SCENE3D_H
#define SCENE3D_H

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Scene.h"
#include "Texture.h"
#include "Model.h"

struct baseMaterialSettings_t
{
	glm::vec4								diffuse;
	glm::vec4								specular;
	glm::vec4								ambient;
	glm::vec4								emissive;
	glm::vec4								reflective;

	baseMaterialSettings_t()
	{
		diffuse = glm::vec4(0.0f);
		specular = glm::vec4(0.0f);
		ambient = glm::vec4(0.0f);
		emissive = glm::vec4(0.0f);
		reflective = glm::vec4(0.0f);
	}
};

class scene3D : public scene
{
public:

	scene3D(const char* windowName = "Ziyad Barakat's Portfolio(3D scene)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 200.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/Model.txt",
		model_t* model = new model_t("../../resources/models/SoulSpear/SoulSpear.fbx")) :
		scene(windowName, camera3D, shaderConfigPath)
	{
		testModel = model;
		wireframe = false;
	}

	virtual ~scene3D() {};

	//override input code. use this to mess with camera
	virtual void SetupCallbacks() override 
	{
		manager->resizeEvent = std::bind(&scene3D::HandleWindowResize, this, _1, _2);
		manager->maximizedEvent = std::bind(&scene3D::HandleMaximize, this, _1);
		//manager->destroyedEvent = std::bind(&scene::ShutDown, this, _1);

		manager->mouseButtonEvent = std::bind(&scene3D::HandleMouseClick, this, _1, _2, _3);
		manager->mouseMoveEvent = std::bind(&scene3D::HandleMouseMotion, this, _1, _2, _3);
		manager->keyEvent = std::bind(&scene3D::HandleKey, this, _1, _2, _3);
	}

	virtual void Initialize() override
	{
		scene::Initialize();
		testModel->loadModel();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

protected:

	model_t* testModel;
	//texture* diffuse;
	bufferHandler_t<baseMaterialSettings_t>	materialBuffer;
	bool wireframe;

	virtual void Draw() override 
	{
		//for each mesh in the model
		for(const auto& iter : testModel->meshes)
		{
			if (iter.isCollision)
			{
				continue;
			}
			//set the materials per mesh
			materialBuffer.data.diffuse = iter.diffuse;
			materialBuffer.data.ambient = iter.ambient;
			materialBuffer.data.specular = iter.specular;
			materialBuffer.data.reflective = iter.reflective;
			materialBuffer.Update(gl_uniform_buffer, gl_dynamic_draw);

			//glBindBuffer(gl_element_array_buffer, iter.indexBufferHandle);
			glBindVertexArray(iter.vertexArrayHandle);
			glUseProgram(this->programGLID);

			/*if (iter.textures.size() > 0)
			{

				iter.textures[0].GetUniformLocation(programGLID);
			}*/
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, iter.indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void Update() override
	{
		//this keeps resetting the values
		//sceneCamera->translation = glm::yawPitchRoll(sceneCamera->rotation.x, sceneCamera->rotation.y, 0.0f);
		//sceneCamera->translation[3] = glm::vec4(sceneCamera->position, 1.0f);

		manager->PollForEvents();
		sceneCamera->Update();
		sceneClock->UpdateClockAdaptive();

		defaultPayload.data.deltaTime = (float)sceneClock->GetDeltaTime();
		defaultPayload.data.totalTime = (float)sceneClock->GetTotalTime();
		defaultPayload.data.framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultPayload.data.totalFrames++;

		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.view = sceneCamera->view;
		if (sceneCamera->currentProjectionType == camera::projection_t::perspective)
		{
			defaultPayload.data.translation = testModel->makeTransform();
		}

		else
		{
			defaultPayload.data.translation = sceneCamera->translation;
		}
		
		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene::BuildGUI(window, io);
		ImGui::Checkbox("wireframe", &wireframe);
		DrawCameraStats();

		//set up the view matrix
		ImGui::Begin("camera transform", &testModel->isGUIActive);
		if (ImGui::CollapsingHeader("view matrix", NULL))
		{
			ImGui::DragFloat4("right", (float*)&sceneCamera->view[0], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("up", (float*)&sceneCamera->view[1], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("forward", (float*)&sceneCamera->view[2], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("position", (float*)&sceneCamera->view[3], 0.1f, -100.0f, 100.0f);
		}
		if (ImGui::CollapsingHeader("projection matrix", NULL))
		{
			ImGui::DragFloat4("projection 0", (float*)&sceneCamera->projection[0], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("projection 1", (float*)&sceneCamera->projection[1], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("projection 2", (float*)&sceneCamera->projection[2], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("projection 3", (float*)&sceneCamera->projection[3], 0.1f, -100.0f, 100.0f);
		}
		if (ImGui::CollapsingHeader("translation matrix", NULL))
		{
			ImGui::DragFloat4("row 0", (float*)&sceneCamera->translation[0], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("row 1", (float*)&sceneCamera->translation[1], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("row 2", (float*)&sceneCamera->translation[2], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("row 3", (float*)&sceneCamera->translation[3], 0.1f, -100.0f, 100.0f);
		}

		ImGui::Text("pitch: %.1f\tyaw: %.1f\troll: %.1f", glm::degrees(sceneCamera->rotator.y), glm::degrees(sceneCamera->rotation.z), glm::degrees(sceneCamera->rotation.x));
		ImGui::End();

	/*	ImGui::Begin("Model", &testModel->isGUIActive, ImVec2(0, 0));
		if (ImGui::CollapsingHeader("translation matrix", NULL))
		{
			ImGui::DragFloat3("position", (float*)&testModel->position, 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat3("rotation", (float*)&testModel->rotation, 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat3("scale", (float*)&testModel->scale, 0.1f, -100.0f, 100.0f);
		}*/

		//ImGui::End();
	}

	virtual void DrawCameraStats() override
	{
		//set up the view matrix
		ImGui::Begin("camera", &isGUIActive);

		ImGui::DragFloat("near plane", &sceneCamera->nearPlane);
		ImGui::DragFloat("far plane", &sceneCamera->farPlane);
		ImGui::SliderFloat("Field of view", &sceneCamera->fieldOfView, 0, 90, "%.0f");

		ImGui::InputFloat("camera speed", &sceneCamera->speed, 0.f);
		ImGui::InputFloat("x sensitivity", &sceneCamera->xSensitivity, 0.f);
		ImGui::InputFloat("y sensitivity", &sceneCamera->ySensitivity, 0.f);
		ImGui::End();
	}

	virtual void InitializeUniforms() override
	{
		defaultPayload.data = defaultUniformBuffer(sceneCamera);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		defaultPayload.data.resolution = glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.translation = sceneCamera->translation;
		defaultPayload.data.view = sceneCamera->view;

		materialBuffer.data = baseMaterialSettings_t();

		defaultPayload.Initialize(0);
		materialBuffer.Initialize(1);
	}

	virtual void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state) override
	{
		scene::HandleMouseClick(window, button, state);
	}

	virtual void HandleMouseMotion(tWindow* window, vec2_t<int> windowPosition, vec2_t<int> screenPosition) override
	{
		scene3D* thisScene = (scene3D*)window->settings.userData;
		scene::HandleMouseMotion(window, windowPosition, screenPosition);

		glm::vec2 mouseDelta = glm::vec2(window->mousePosition.x - window->previousMousePosition.x, window->mousePosition.y - window->previousMousePosition.y);
		float deltaTime = (float)thisScene->sceneClock->GetDeltaTime();
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
		glViewport(0, 0, window->settings.resolution.width, window->settings.resolution.height);
		sceneCamera->resolution = glm::vec2(window->settings.resolution.width, window->settings.resolution.height);
		defaultPayload.data.resolution = sceneCamera->resolution;
		sceneCamera->UpdateProjection();
		defaultPayload.data.projection = sceneCamera->projection;

		//bind the uniform buffer and refill it
		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		scene3D* thisScene = (scene3D*)window->settings.userData;
		glViewport(0, 0, dimensions.width, dimensions.height);
		sceneCamera->resolution = glm::vec2(dimensions.width, dimensions.height);
		defaultPayload.data.resolution = sceneCamera->resolution;
		sceneCamera->UpdateProjection();
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.deltaTime = (float)sceneClock->GetDeltaTime();
		defaultPayload.data.totalTime = (float)sceneClock->GetTotalTime();
		defaultPayload.data.framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());

		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
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
			if(window->keys['w'] == keyState_t::down)
			{
				sceneCamera->MoveForward(camSpeed, deltaTime);
			}

			if (window->keys['s'] == keyState_t::down)
			{
				sceneCamera->MoveForward(-camSpeed, deltaTime);
			}

			if (window->keys['a'] == keyState_t::down)
			{
				sceneCamera->MoveRight(-camSpeed, deltaTime);
			}

			if (window->keys['d'] == keyState_t::down)
			{
				sceneCamera->MoveRight(camSpeed, deltaTime);
			}

			if (window->keys['e'] == keyState_t::down)
			{
				sceneCamera->MoveUp(camSpeed, deltaTime);
			}

			if (window->keys['q'] == keyState_t::down)
			{
				sceneCamera->MoveUp(-camSpeed, deltaTime);
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
	}};

#endif

