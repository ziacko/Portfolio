#ifndef EROSION_H
#define EROSION_H
#include "Scene3D.h"
#include "FrameBuffer.h"

class framebufferScene : public scene3D
{
public:

	framebufferScene(
		const char* windowName = "Ziyad Barakat's portfolio (Framebuffer)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/FrameBuffer.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(gl_generate_mipmap_hint, GL_NICEST);

		geometryBuffer = new frameBuffer();
	}

	~framebufferScene() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"color", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::depth,
			"depth", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height),
			GL_DEPTH_COMPONENT, GL_TEXTURE_2D, GL_FLOAT, gl_depth_component32f));

		frameBuffer::Unbind();
		compareProgram = shaderPrograms[1]->handle;
		finalProgram = shaderPrograms[2]->handle;
	}

protected:

	frameBuffer* geometryBuffer;

	unsigned int finalProgram = 0;
	unsigned int compareProgram = 0;

	bool enableCompare = true;

	virtual void Update() override
	{
		manager->PollForEvents();
		if (lockedFrameRate > 0)
		{
			sceneClock->UpdateClockFixed(lockedFrameRate);
		}
		else
		{
			sceneClock->UpdateClockAdaptive();
		}

		defaultUniform->deltaTime = (float)sceneClock->GetDeltaTime();
		defaultUniform->totalTime = (float)sceneClock->GetTotalTime();
		defaultUniform->framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultUniform->totalFrames++;

		defaultVertexBuffer->UpdateBuffer(defaultUniform->resolution);
	}

	void UpdateDefaultBuffer()
	{
		sceneCamera->UpdateProjection();
		defaultUniform->projection = sceneCamera->projection;
		defaultUniform->view = sceneCamera->view;
		if (sceneCamera->currentProjectionType == camera::projection_t::perspective)
		{
			defaultUniform->translation = testModel->makeTransform();
		}

		else
		{
			defaultUniform->translation = sceneCamera->translation;
		}
		defaultUniform->deltaTime = (float)sceneClock->GetDeltaTime();
		defaultUniform->totalTime = (float)sceneClock->GetTotalTime();
		defaultUniform->framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());

		UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
		defaultVertexBuffer->UpdateBuffer(defaultUniform->resolution);
	}

	void Draw() override
	{
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		GeometryPass(); //render current scene with jitter

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();

		FinalPass(geometryBuffer->attachments[0], geometryBuffer->attachments[1]);

		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void GeometryPass()
	{
		geometryBuffer->Bind();

		GLenum drawbuffers[1] = {
			geometryBuffer->attachments[0]->attachmentFormat, //color
		};

		glDrawBuffers(1, drawbuffers);

		//we just need the first LOd so only do the first 3 meshes
		for (size_t iter = 0; iter < 3; iter++)
		{
			if (testModel->meshes[iter].isCollision)
			{
				continue;
			}

			testModel->meshes[iter].textures[0].SetActive(0);
			//add the previous depth?

			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(programGLID);
			glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);

			//glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		geometryBuffer->Unbind();
	}

	void FinalPass(texture* tex1, texture* tex2)
	{
		//draw directly to backbuffer		
		tex1->SetActive(0);

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
		if (enableCompare)
		{
			tex2->SetActive(1);
			glUseProgram(compareProgram);
		}

		else
		{
			glUseProgram(finalProgram);
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(windows[0], io);

		DrawBufferAttachments();
	}

	virtual void DrawBufferAttachments()
	{
		ImGui::Begin("framebuffers");
		ImGui::Checkbox("enable Compare", &enableCompare);
		for (auto iter : geometryBuffer->attachments)
		{
			ImGui::Image((ImTextureID)iter->GetHandle(), ImVec2(512, 288),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Text("%s\n", iter->GetUniformName().c_str());
		}

		ImGui::End();
	}

	virtual void DrawCameraStats() override
	{
		//set up the view matrix
		ImGui::Begin("camera", &isGUIActive, ImVec2(0, 0));

		ImGui::DragFloat("near plane", &sceneCamera->nearPlane);
		ImGui::DragFloat("far plane", &sceneCamera->farPlane);
		ImGui::SliderFloat("Field of view", &sceneCamera->fieldOfView, 0, 90, "%.0f");

		ImGui::InputFloat("camera speed", &sceneCamera->speed, 0.f);
		ImGui::InputFloat("x sensitivity", &sceneCamera->xSensitivity, 0.f);
		ImGui::InputFloat("y sensitivity", &sceneCamera->ySensitivity, 0.f);
		ImGui::End();
	}

	virtual void ClearBuffers()
	{
		//ok copy the current frame into the previous frame and clear the rest of the buffers	
		float clearColor1[4] = { 0.25f, 0.25f, 0.25f, 0.25f };

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		geometryBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::vec2 resolution)
	{
		geometryBuffer->Resize(resolution);
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		defaultUniform->resolution = glm::vec2(dimensions.width, dimensions.height);
		ResizeBuffers(glm::vec2(dimensions.x, dimensions.y));
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		defaultUniform->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		ResizeBuffers(defaultUniform->resolution);
	}

	virtual void InitializeUniforms() override
	{
		defaultUniform = new defaultUniformBuffer(sceneCamera);
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);

		defaultUniform->resolution = glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height);
		defaultUniform->projection = sceneCamera->projection;
		defaultUniform->translation = sceneCamera->translation;
		defaultUniform->view = sceneCamera->view;

		SetupVertexBuffer();
		SetupBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), 0, gl_uniform_buffer, gl_dynamic_draw);	
	}
};

#endif