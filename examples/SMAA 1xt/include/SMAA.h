#ifndef SMAA_H
#define SMAA_H

#include "Scene3D.h"
#include "FrameBuffer.h"

class SMAA : public scene3D
{
public:

	SMAA(
		const char* windowName = "Ziyad Barakat's portfolio (SMAA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 100.0f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/SMAA.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glHint(gl_generate_mipmap_hint, GL_NICEST);

		geometryBuffer = new frameBuffer();
		edgesBuffer = new frameBuffer();
		weightsBuffer = new frameBuffer();
		SMAABuffer = new frameBuffer();

		SMAAArea = new texture("../../resources/textures/SMAA/AreaTexDX10.dds");
		SMAASearch = new texture("../../resources/textures/SMAA/SearchTex.dds");
	}

	~SMAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();
		SMAAArea->LoadTexture();
		SMAASearch->LoadTexture();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		FBODescriptor depthDesc;
		depthDesc.dataType = GL_FLOAT;
		depthDesc.format = GL_DEPTH_COMPONENT;
		depthDesc.internalFormat = gl_depth_component24;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depth;

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height),
			depthDesc));

		edgesBuffer->Initialize();
		edgesBuffer->Bind();

		FBODescriptor edgeDesc;
		edgeDesc.format = gl_rg;
		edgeDesc.internalFormat = gl_rg8;

		edgesBuffer->AddAttachment(new frameBuffer::attachment_t("edge",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height), edgeDesc));

		weightsBuffer->Initialize();
		weightsBuffer->Bind();

		weightsBuffer->AddAttachment(new frameBuffer::attachment_t("blend",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		SMAABuffer->Initialize();
		SMAABuffer->Bind();
		SMAABuffer->AddAttachment(new frameBuffer::attachment_t("SMAA",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		edgeDetectionProgram = shaderPrograms[1]->handle;
		blendingWeightProgram = shaderPrograms[2]->handle;
		SMAAProgram = shaderPrograms[3]->handle;
		compareProgram = shaderPrograms[4]->handle;
		finalProgram = shaderPrograms[5]->handle;
		
		frameBuffer::Unbind();
	}

protected:

	frameBuffer*					geometryBuffer;
	frameBuffer*					edgesBuffer;
	frameBuffer*					weightsBuffer;
	frameBuffer*					SMAABuffer;

	texture*						SMAAArea;
	texture*						SMAASearch;

	unsigned int edgeDetectionProgram = 0;
	unsigned int blendingWeightProgram = 0;
	unsigned int SMAAProgram = 0;
	unsigned int compareProgram = 0;
	unsigned int finalProgram = 0;

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

	virtual void Draw()
	{
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		JitterPass(); //render current scene with jitter

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();
		
		EdgeDetectionPass();
		BlendingWeightsPass();

		SMAAPass();

		FinalPass(SMAABuffer->attachments[0], geometryBuffer->attachments[0]);
		
		DrawGUI(windows[0]);
		
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void JitterPass()
	{
		geometryBuffer->Bind();

		GLenum drawbuffers[1] = {
			geometryBuffer->attachments[0]->FBODesc.attachmentFormat, //color
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

			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(this->programGLID);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		geometryBuffer->Unbind();
	}

	virtual void EdgeDetectionPass()
	{
		edgesBuffer->Bind();

		glDrawBuffers(1, &edgesBuffer->attachments[0]->FBODesc.attachmentFormat);

		geometryBuffer->attachments[0]->SetActive(0);//color
		geometryBuffer->attachments[1]->SetActive(1);//depth

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(edgeDetectionProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		edgesBuffer->Unbind();
	}

	virtual void BlendingWeightsPass()
	{
		weightsBuffer->Bind();

		glDrawBuffers(1, &weightsBuffer->attachments[0]->FBODesc.attachmentFormat);

		edgesBuffer->attachments[0]->SetActive(0);
		SMAAArea->SetActive(1);
		SMAASearch->SetActive(2);

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(blendingWeightProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		weightsBuffer->Unbind();
	}

	virtual void SMAAPass()
	{
		SMAABuffer->Bind();
		glDrawBuffers(1, &SMAABuffer->attachments[0]->FBODesc.attachmentFormat);

		//current frame
		geometryBuffer->attachments[0]->SetActive(0); // color
		weightsBuffer->attachments[0]->SetActive(1); //blending weights

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(SMAAProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		SMAABuffer->Unbind();
	}

	void FinalPass(texture* tex1, texture* tex2)
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

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(windows[0], io);

		DrawBufferAttachments();
		DrawSMAASettings();
	}

	void DrawSMAASettings()
	{
		ImGui::Begin("SMAA Settings");
		ImGui::Checkbox("enable Compare", &enableCompare);

		ImGui::End();
	}

	virtual void DrawBufferAttachments()
	{
		ImGui::Begin("framebuffers");
		for (auto iter : geometryBuffer->attachments)
		{
			ImGui::Image((ImTextureID*)iter->GetHandle(), ImVec2(512, 288),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Text("%s\n", iter->GetUniformName().c_str());
		}

		ImGui::Image((ImTextureID*)edgesBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", edgesBuffer->attachments[0]->GetUniformName().c_str());

		ImGui::Image((ImTextureID*)weightsBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", weightsBuffer->attachments[0]->GetUniformName().c_str());

		ImGui::Image((ImTextureID*)SMAABuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", SMAABuffer->attachments[0]->GetUniformName().c_str());

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
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		geometryBuffer->Unbind();

		SMAABuffer->Bind();
		SMAABuffer->ClearTexture(SMAABuffer->attachments[0], clearColor1);
		SMAABuffer->Unbind();

		edgesBuffer->Bind();
		edgesBuffer->ClearTexture(edgesBuffer->attachments[0], clearColor2);
		edgesBuffer->Unbind();

		weightsBuffer->Bind();
		weightsBuffer->ClearTexture(weightsBuffer->attachments[0], clearColor2);
		weightsBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::vec2 resolution)
	{
		for (auto iter : geometryBuffer->attachments)
		{
			iter->Resize(resolution);
		}

		edgesBuffer->attachments[0]->Resize(resolution);
		weightsBuffer->attachments[0]->Resize(resolution);
		SMAABuffer->attachments[0]->Resize(resolution);
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		defaultPayload.data.resolution = glm::vec2(dimensions.width, dimensions.height);
		ResizeBuffers(glm::vec2(dimensions.x, dimensions.y));
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		defaultPayload.data.resolution = glm::vec2(window->settings.resolution.width, window->settings.resolution.height);
		ResizeBuffers(defaultPayload.data.resolution);
	}

	virtual void InitializeUniforms() override
	{
		defaultPayload = bufferHandler_t<defaultUniformBuffer>(sceneCamera);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		defaultPayload.data.resolution = glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.translation = sceneCamera->translation;
		defaultPayload.data.view = sceneCamera->view;

		defaultPayload.Initialize(0);

		SetupVertexBuffer();
	}
};
#endif