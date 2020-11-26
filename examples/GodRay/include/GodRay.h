#pragma once

#include "Scene3D.h"
#include "FrameBuffer.h"


struct godRaySettings_t
{
	glm::vec4 lightPosition;
	float decay;
	float density;
	float weight;
	float exposure;
	int samples;	

	godRaySettings_t()
	{
		exposure = 1.0f;
		density = 0.0001f;
		weight = 0.05f;
		decay = 0.75f;
		lightPosition = glm::vec4(0);
		samples = 20;
	}

	~godRaySettings_t() {};

};


class GodRayScene : public scene3D
{
public:

	GodRayScene(
		const char* windowName = "Ziyad Barakat's portfolio (god ray test)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/GodRay.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		geometryBuffer = new frameBuffer();
		occlusionBuffer = new frameBuffer();

		//orthoCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::orthographic, 0.1f);
		godRay = bufferHandler_t<godRaySettings_t>();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glHint(gl_generate_mipmap_hint, GL_NICEST);
	}

	virtual ~GodRayScene() {};

	void Initialize() override
	{
		scene3D::Initialize();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		FBODescriptor geomDesc;
		geomDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		FBODescriptor depthDesc;
		depthDesc.dataType = GL_FLOAT;
		depthDesc.format = GL_DEPTH_COMPONENT;
		depthDesc.internalFormat = gl_depth_component24;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depth;
		depthDesc.wrapRSetting = GL_CLAMP;
		depthDesc.wrapSSetting = GL_CLAMP;
		depthDesc.wrapTSetting = GL_CLAMP;
		depthDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color", geomDesc));
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth", depthDesc));

		occlusionBuffer->Initialize();
		occlusionBuffer->Bind();

		FBODescriptor occlusionDesc;
		occlusionDesc.format = GL_RED;
		occlusionDesc.internalFormat = gl_r8;
		occlusionDesc.dataType = GL_UNSIGNED_BYTE;
		occlusionDesc.wrapRSetting = GL_CLAMP;
		occlusionDesc.wrapSSetting = GL_CLAMP;
		occlusionDesc.wrapTSetting = GL_CLAMP;
		occlusionDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		occlusionBuffer->AddAttachment(new frameBuffer::attachment_t("occlusion", occlusionDesc));

		frameBuffer::Unbind();

		occlusionProgram = shaderPrograms[1]->handle;
		GodRayPostProgram = shaderPrograms[2]->handle;
		finalProgram = shaderPrograms[3]->handle;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glDisable(GL_BLEND);
		//glEnable(GL_DEPTH_TEST);
		//glEnable(gl_clip_distance0);
		//glDepthFunc(GL_LESS);
		//glHint(gl_generate_mipmap_hint, GL_NICEST);;
	}

protected:

	bufferHandler_t<godRaySettings_t> godRay;

	frameBuffer* geometryBuffer;
	frameBuffer* occlusionBuffer;

	//camera* orthoCamera;

	unsigned int GodRayPostProgram = 0;
	unsigned int finalProgram = 0;
	unsigned int occlusionProgram = 0;

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

		defaultPayload.data.deltaTime = static_cast<float>(sceneClock->GetDeltaTime());
		defaultPayload.data.totalTime = static_cast<float>(sceneClock->GetTotalTime());
		defaultPayload.data.framesPerSec = static_cast<float>(1.0 / sceneClock->GetDeltaTime());
		defaultPayload.data.totalFrames++;

		defaultVertexBuffer->UpdateBuffer(defaultPayload.data.resolution);

		godRay.Update();
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

		//occlusion pass
		OcclusionPass();

		//render scene normally
		GeometryPass();

		//swap to orthographic
		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();	

		//final pass
		GodRayPass();

		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void GeometryPass()
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
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		geometryBuffer->Unbind();
	}

	virtual void OcclusionPass()
	{
		glDisable(GL_BLEND);
		occlusionBuffer->Bind();

		GLenum drawbuffers[1] = {
			occlusionBuffer->attachments[0]->FBODesc.attachmentFormat, //occlusion
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
			glUseProgram(occlusionProgram);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		occlusionBuffer->Unbind();
	}

	virtual void GodRayPass()
	{
		
		//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		//just draw to backbuffer. we don't need to compare anything
		geometryBuffer->attachments[0]->SetActive(0);
		occlusionBuffer->attachments[0]->SetActive(1);

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		glUseProgram(GodRayPostProgram);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void FinalPass(texture* tex1, texture* tex2)
	{
		//draw directly to backbuffer		
		tex1->SetActive(0);

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		glUseProgram(finalProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(windows[0], io);

		DrawBufferAttachments();
		DrawGodRaySettings();
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

		ImGui::Image((ImTextureID)occlusionBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", occlusionBuffer->attachments[0]->GetUniformName().c_str());

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

	virtual void DrawGodRaySettings()
	{
		ImGui::Begin("God rays");
		ImGui::InputFloat("decay", &godRay.data.decay);
		ImGui::InputFloat("density", &godRay.data.density);
		ImGui::InputFloat("weight", &godRay.data.weight);
		ImGui::InputFloat("exposure", &godRay.data.exposure);
		ImGui::InputInt("samples", &godRay.data.samples);

		ImGui::End();
	}

	virtual void ClearBuffers()
	{
		//ok copy the current frame into the previous frame and clear the rest of the buffers	
		float clearColor1[4] = { 0.25f, 0.25f, 0.25f, 0.25f };
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		//frameBuffer::Unbind();

		occlusionBuffer->Bind();
		occlusionBuffer->ClearTexture(occlusionBuffer->attachments[0], clearColor2);
		frameBuffer::Unbind();
	}

	virtual void ResizeBuffers(glm::ivec2 resolution)
	{
		geometryBuffer->Resize(glm::ivec3(resolution, 1));
		occlusionBuffer->Resize(glm::ivec3(resolution, 1));
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		defaultPayload.data.resolution = glm::ivec2(dimensions.width, dimensions.height);
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
		godRay.Initialize(1);
	}
};