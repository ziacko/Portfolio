#ifndef SSAA_H
#define SSAA_H

#include "Scene3D.h"
#include "FrameBuffer.h"

using downsampleType_t = enum { none = 0, lanczos };

struct jitterSettings_t
{
	glm::vec2			haltonSequence[128];
	float				haltonScale;
	int					haltonIndex;
	int					enableDithering;
	float				ditheringScale;

	jitterSettings_t()
	{
		haltonIndex = 16;
		enableDithering = 1;
		haltonScale = 100.0f;
		ditheringScale = 0.0f;
	}

	~jitterSettings_t() {};
};

struct downscaleSettings_t
{
	float texelWidthOffset;
	float texelHeightOffset;
	int downsampleMode;

	downscaleSettings_t()
	{
		texelWidthOffset = 0.00025f;
		texelHeightOffset = 0.00025f;
		downsampleMode = none;
	}
};

struct lanzcosSettings_t
{
	float magicValue1;
	float magicValue2;
	float magicValue3;
	float magicValue4;
	float magicValue5;

	lanzcosSettings_t()
	{
		magicValue1 = 0.38026f;
		magicValue2 = 0.27667f;
		magicValue3 = 0.08074f;
		magicValue4 = -0.02612f;
		magicValue5 = -0.02143f;
	}
};

class SSAA : public scene3D
{
public:

	SSAA(
		const char* windowName = "Ziyad Barakat's portfolio (SSAA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/SSAA.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glHint(gl_generate_mipmap_hint, GL_NICEST);

		downscaledBuffer = new frameBuffer();
		geometryBuffer = new frameBuffer();
		unJitteredBuffer = new frameBuffer();

		jitterUniforms = bufferHandler_t<jitterSettings_t>();
		downscaleUniforms = bufferHandler_t<downscaleSettings_t>();
		lanzcosUniforms = bufferHandler_t<lanzcosSettings_t>();

		for (int iter = 0; iter < 128; iter++)
		{
			jitterUniforms.data.haltonSequence[iter] = glm::vec2(CreateHaltonSequence(iter + 1, 2), CreateHaltonSequence(iter + 1, 3));
		}
	}

	~SSAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color", 
			glm::vec2(windows[0]->settings.resolution.width * 2, windows[0]->settings.resolution.height * 2)));

		FBODescriptor depthDesc;
		depthDesc.dataType = GL_FLOAT;
		depthDesc.format = GL_DEPTH_COMPONENT;
		depthDesc.internalFormat = gl_depth_component24;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depth;

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth", 
			glm::vec2(windows[0]->settings.resolution.width * 2, windows[0]->settings.resolution.height * 2),
			depthDesc));

		downscaledBuffer->Initialize();
		downscaledBuffer->Bind();
		downscaledBuffer->AddAttachment(new frameBuffer::attachment_t(("downscaled"),
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		unJitteredBuffer->Initialize();
		unJitteredBuffer->Bind();
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t("unJittered",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t("depth", 
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height),
			depthDesc));

		//geometry automatically gets assigned to 0
		downscaleProgram = shaderPrograms[1]->handle;
		unjitteredProgram = shaderPrograms[2]->handle;
		compareProgram = shaderPrograms[3]->handle;
		finalProgram = shaderPrograms[4]->handle;

		frameBuffer::Unbind();
	}

protected:

	frameBuffer* geometryBuffer;
	frameBuffer* downscaledBuffer;
	frameBuffer* unJitteredBuffer;

	unsigned int downscaleProgram = 0;
	unsigned int unjitteredProgram = 0;
	unsigned int compareProgram = 0;
	unsigned int finalProgram = 0;

	bool enableCompare = true;
	bufferHandler_t<jitterSettings_t>		jitterUniforms;
	bufferHandler_t<downscaleSettings_t>	downscaleUniforms;
	bufferHandler_t<lanzcosSettings_t>		lanzcosUniforms;

	std::vector<const char*>		downsampleSettings = { "none", "lanczos" };

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

		downscaleUniforms.Update();
		jitterUniforms.Update();
		lanzcosUniforms.Update();
	}

	void UpdateDefaultBuffer(glm::vec2 resolution)
	{
		sceneCamera->UpdateProjection();
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.view = sceneCamera->view;
		defaultPayload.data.resolution = resolution;
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
		glm::vec2 defaultResolution = sceneCamera->resolution;
		sceneCamera->resolution = glm::vec2(sceneCamera->resolution.x * 2, sceneCamera->resolution.y * 2);
		sceneCamera->Update();

		UpdateDefaultBuffer(sceneCamera->resolution);

		JitterPass(); //render current scene with jitter
		sceneCamera->resolution = defaultResolution;
		if (enableCompare)
		{
			UnJitteredPass();
		}

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer(defaultResolution);
		
		DownscalePass(); //use the positions, colors, depth and velocity to smooth the final image

		FinalPass(downscaledBuffer->attachments[0], unJitteredBuffer->attachments[0]);
		
		DrawGUI(windows[0]);
		
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//sceneCamera->resolution = defaultResolution;
	}

	virtual void JitterPass()
	{
		geometryBuffer->Bind();

		GLenum drawbuffers[1] = {
			geometryBuffer->attachments[0]->FBODesc.attachmentFormat, //color
		};

		glDrawBuffers(1, drawbuffers);

		//we just need the first LOD so only do the first 3 meshes
		for (size_t iter = 0; iter < 3; iter++)
		{
			if (testModel->meshes[iter].isCollision)
			{
				continue;
			}

			testModel->meshes[iter].textures[0].SetActive(0);

			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(this->programGLID);
			glViewport(0, 0, windows[0]->settings.resolution.width * 2, windows[0]->settings.resolution.height * 2);
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		geometryBuffer->Unbind();
	}

	virtual void UnJitteredPass()
	{
		unJitteredBuffer->Bind();

		GLenum drawbuffers[1] = {
			unJitteredBuffer->attachments[0]->FBODesc.attachmentFormat, //color
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
			glUseProgram(unjitteredProgram);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		unJitteredBuffer->Unbind();
	}

	virtual void DownscalePass()
	{
		downscaledBuffer->Bind();
		GLenum drawBuffers[1] = {
			downscaledBuffer->attachments[0]->FBODesc.attachmentFormat
		};
		glDrawBuffers(1, drawBuffers);

		//current frame
		geometryBuffer->attachments[0]->SetActive(0); //current color
		
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(downscaleProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		downscaledBuffer->Unbind();
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
		DrawTAASettings();
		DrawDownsampleSettings();
	}

	virtual void DrawTAASettings()
	{
		ImGui::Begin("TAA Settings");
		ImGui::Text("performance | %f", defaultTimer->GetTimeMilliseconds());
		ImGui::Checkbox("enable Compare", &enableCompare);

		//jitter settings
		ImGui::Separator();
		ImGui::DragFloat("halton scale", &jitterUniforms.data.haltonScale, 0.1f, 0.0f, 1000.0f, "%.3f");
		ImGui::DragInt("halton index",  &jitterUniforms.data.haltonIndex, 1.0f, 0, 128);
		ImGui::DragInt("enable dithering", &jitterUniforms.data.enableDithering, 1.0f, 0, 1);
		ImGui::DragFloat("dithering scale", &jitterUniforms.data.ditheringScale, 1.0f, 0.0f, 1000.0f, "%.3f");

		ImGui::End();
	}

	virtual void DrawDownsampleSettings()
	{
		ImGui::Begin("Lanczos settings");
		ImGui::ListBox("downsample types", &downscaleUniforms.data.downsampleMode, downsampleSettings.data(), downsampleSettings.size());
		ImGui::DragFloat("texel width", &downscaleUniforms.data.texelWidthOffset, 0.1f, 0.0f, 10.0f, "%.5f");
		ImGui::DragFloat("texel height", &downscaleUniforms.data.texelHeightOffset, 0.1f, 0.0f, 10.0f, "%.5f");
		switch(downscaleUniforms.data.downsampleMode)
		{
		case none:
		{
			break;
		}

		case lanczos:
		{
			
			ImGui::DragFloat("value 1", &lanzcosUniforms.data.magicValue1, 0.01f, 0.0f, 1.0f, "%.3f");
			ImGui::DragFloat("value 2", &lanzcosUniforms.data.magicValue2, 0.01f, 0.0f, 1.0f, "%.3f");
			ImGui::DragFloat("value 3", &lanzcosUniforms.data.magicValue3, 0.01f, 0.0f, 1.0f, "%.3f");
			ImGui::DragFloat("value 4", &lanzcosUniforms.data.magicValue4, 0.01f, 0.0f, 1.0f, "%.3f");
			ImGui::DragFloat("value 5", &lanzcosUniforms.data.magicValue5, 0.01f, 0.0f, 1.0f, "%.3f");

			break;
		}

		default:
		{
			break;
		}

		}

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

		ImGui::Image((ImTextureID*)downscaledBuffer->attachments[0]->GetHandle(), ImVec2(512, 288), 
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", downscaledBuffer->attachments[0]->GetUniformName().c_str());

		ImGui::Image((ImTextureID*)unJitteredBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", unJitteredBuffer->attachments[0]->GetUniformName().c_str());

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
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float clearColor3[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; //this is for debugging only!

		downscaledBuffer->Bind(); //clear the previous, the next frame current becomes previous
		downscaledBuffer->ClearTexture(downscaledBuffer->attachments[0], clearColor1);
		downscaledBuffer->Unbind();

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		geometryBuffer->ClearTexture(geometryBuffer->attachments[1], clearColor2);
		glClear(GL_DEPTH_BUFFER_BIT);
		geometryBuffer->Unbind();

		unJitteredBuffer->Bind();
		unJitteredBuffer->ClearTexture(unJitteredBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		unJitteredBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::vec2 resolution)
	{
		for (auto iter : geometryBuffer->attachments)
		{
			iter->Resize(glm::vec2(resolution.x * 2, resolution.y * 2));
		}

		downscaledBuffer->attachments[0]->Resize(resolution);
		unJitteredBuffer->attachments[0]->Resize(resolution);
		unJitteredBuffer->attachments[1]->Resize(resolution);
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
		jitterUniforms.Initialize(1);
		downscaleUniforms.Initialize(2);
		lanzcosUniforms.Initialize(3);

		SetupVertexBuffer();
	}

	float CreateHaltonSequence(unsigned int index, int base)
	{
		float f = 1;
		float r = 0;
		int current = index;
		do 
		{
			f = f / base;
			r = r + f * (current % base);
			current = glm::floor(current / base);
		} while (current > 0);

		return r;
	}
};

#endif