#ifndef STENCIL_H
#define STENCIL_H
#include "Scene3D.h"
#include "FrameBuffer.h"

class stencil : public scene3D
{
public:

	stencil(const char* windowName = "Ziyad Barakat's Portfolio(stencil test)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 10.0f, camera::projection_t::perspective),
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"),
		const char* shaderConfigPath = "../../resources/shaders/Stencil.txt") : 
		scene3D(windowName, camera3D, shaderConfigPath)
	{
		testModel = model;
		geometryBuffer = new frameBuffer();
	}

	~stencil() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		FBODescriptor colorDesc;
		colorDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		FBODescriptor depthDesc;
		depthDesc.dataType = gl_unsigned_int_24_8;
		depthDesc.format = gl_depth_stencil;
		depthDesc.internalFormat = gl_depth24_stencil8;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depthAndStencil;
		depthDesc.wrapRSetting = GL_CLAMP;
		depthDesc.wrapSSetting = GL_CLAMP;
		depthDesc.wrapTSetting = GL_CLAMP;
		depthDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		FBODescriptor stencilDesc;
		stencilDesc.dataType = GL_UNSIGNED_INT;
		stencilDesc.format = GL_STENCIL_INDEX;
		stencilDesc.internalFormat = gl_stencil_index8;
		stencilDesc.attachmentType = FBODescriptor::attachmentType_t::stencil;
		stencilDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color", colorDesc));
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth", depthDesc));
	
		frameBuffer::Unbind();

		DepthStencilProgram = shaderPrograms[1]->handle;
		compareProgram = shaderPrograms[2]->handle;
		finalProgram = shaderPrograms[3]->handle;
	}

protected:

	frameBuffer* geometryBuffer;

	unsigned int DepthStencilProgram = 0;
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

		defaultPayload.data.deltaTime = (float)sceneClock->GetDeltaTime();
		defaultPayload.data.totalTime = (float)sceneClock->GetTotalTime();
		defaultPayload.data.framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultPayload.data.totalFrames++;

		defaultVertexBuffer->UpdateBuffer(defaultPayload.data.resolution);
	}

	void UpdateDefaultBuffer()
	{
		sceneCamera->UpdateProjection();
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
		defaultPayload.data.deltaTime = (float)sceneClock->GetDeltaTime();
		defaultPayload.data.totalTime = (float)sceneClock->GetTotalTime();
		defaultPayload.data.framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());

		defaultPayload.Update();

		defaultVertexBuffer->UpdateBuffer(defaultPayload.data.resolution);
	}

	void Draw() override
	{
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		EarlyDepthPass();

		GeometryPass(); //render current scene with jitter

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();

		FinalPass(geometryBuffer->attachments[0], geometryBuffer->attachments[1]);

		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void EarlyDepthPass()
	{
		geometryBuffer->Bind();
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 5, 0xFF);
		//glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 1, 0xff);
		
		geometryBuffer->attachments[1]->Draw();

		//we just need the first LOd so only do the first 3 meshes
		for (size_t iter = 0; iter < 3; iter++)
		{
			if (testModel->meshes[iter].isCollision)
			{
				continue;
			}

			testModel->meshes[iter].textures[0].SetActive(0);

			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(DepthStencilProgram);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		geometryBuffer->Unbind();
	}

	virtual void GeometryPass()
	{
		geometryBuffer->Bind();
		geometryBuffer->attachments[0]->Draw();

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
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

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

	virtual void FinalPass(texture* tex1, frameBuffer::attachment_t* tex2)
	{
		//draw directly to backbuffer		
		tex1->SetActive(0);

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
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
		ImGui::Begin("camera", &isGUIActive);

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
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		geometryBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::ivec2 resolution)
	{
		geometryBuffer->Resize(glm::ivec3(resolution, 1));
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		defaultPayload.data.resolution = glm::vec2(dimensions.width, dimensions.height);
		ResizeBuffers(glm::ivec2(dimensions.x, dimensions.y));
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		defaultPayload.data.resolution = glm::ivec2(window->settings.resolution.width, window->settings.resolution.height);
		ResizeBuffers(defaultPayload.data.resolution);
	}

	virtual void InitializeUniforms() override
	{
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		defaultPayload.data.resolution = glm::ivec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.translation = sceneCamera->translation;
		defaultPayload.data.view = sceneCamera->view;

		SetupVertexBuffer();
		defaultPayload.Initialize(0);
	}
};

#endif

