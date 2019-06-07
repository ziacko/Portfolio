#ifndef TEMPORALAA_H
#define TEMPORALAA_H

#include "Scene3D.h"
#include "FrameBuffer.h"

class MSAA : public scene3D
{
public:

	MSAA(
		const char* windowName = "Ziyad Barakat's portfolio (MSAA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/MSAA.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glHint(gl_generate_mipmap_hint, GL_NICEST);

		geometryBuffer = new frameBuffer();
	}

	~MSAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		glEnable(gl_multisample);

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"color", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height), texture::textureType_t::image,
			GL_RGBA, gl_texture_2d_multisample, 8));
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::depth,
			"depth", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height), texture::textureType_t::image,
			GL_DEPTH_COMPONENT, gl_texture_2d_multisample, 8, 0, 0, 0, GL_FLOAT, gl_depth_component24));

		finalProgram = shaderPrograms[1]->handle;
		
		frameBuffer::Unbind();
	}

protected:

	frameBuffer* geometryBuffer;
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

		defaultUniform->deltaTime = (float)sceneClock->GetDeltaTime();
		defaultUniform->totalTime = (float)sceneClock->GetTotalTime();
		defaultUniform->framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultUniform->totalFrames++;
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

	virtual void Draw()
	{
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		GeometryPass(); //render current scene with jitter

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();

		FinalPass(geometryBuffer->attachments[0]);
		
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
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		geometryBuffer->Unbind();
	}

	void FinalPass(texture* tex1)
	{
		//draw directly to backbuffer		
		tex1->SetActive(3);
		
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
		glUseProgram(finalProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(windows[0], io);

		DrawBufferAttachments();
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
		for (auto iter : geometryBuffer->attachments)
		{
			iter->Resize(resolution);
		}
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

		SetupBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), 0, gl_uniform_buffer, gl_dynamic_draw);
		SetupVertexBuffer();
	}
};
#endif