#ifndef SMAA2XT_H
#define SMAA2XT_H

#include "SMAA1xt.h"

struct downscaleSettings_t
{
	float texelWidthOffset;
	float texelHeightOffset;

	downscaleSettings_t()
	{
		texelWidthOffset = 0.00025f;
		texelHeightOffset = 0.00025f;
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

class SMAA2xt : public SMAA1xt
{
public:

	SMAA2xt(
		const char* windowName = "Ziyad Barakat's portfolio (SMAA 2xt)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 100.0f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/SMAA1xt.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: SMAA1xt(windowName, texModelCamera, shaderConfigPath, model)
	{
		velocityUniforms = bufferHandler_t<reprojectSettings_t>();
		taaUniforms = bufferHandler_t<TAASettings_t>();
		jitterUniforms = bufferHandler_t<jitterSettings_t>();
		jitter2Uniforms = bufferHandler_t<jitter2Settings_t>();

		for (int iter = 0; iter < 128; iter++)
		{
			jitterUniforms.data.haltonSequence[iter] = glm::vec2(CreateHaltonSequence(iter + 1, 2), CreateHaltonSequence(iter + 1, 3));
		}
	}

	~SMAA2xt() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();
		SMAAArea->LoadTexture();
		SMAASearch->LoadTexture();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color"));

		FBODescriptor velDesc;
		velDesc.format = gl_rg;
		velDesc.internalFormat = gl_rg16_snorm;
		velDesc.dataType = GL_FLOAT;
		velDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("velocity", velDesc));

		FBODescriptor depthDesc;
		depthDesc.dataType = GL_FLOAT;
		depthDesc.format = GL_DEPTH_COMPONENT;
		depthDesc.internalFormat = gl_depth_component24;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depth;
		depthDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth", depthDesc));

		edgesBuffer->Initialize();
		edgesBuffer->Bind();

		FBODescriptor edgeDesc;
		edgeDesc.format = gl_rg;
		edgeDesc.internalFormat = gl_rg8;
		edgeDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		edgesBuffer->AddAttachment(new frameBuffer::attachment_t("edge", edgeDesc));

		weightsBuffer->Initialize();
		weightsBuffer->Bind();

		weightsBuffer->AddAttachment(new frameBuffer::attachment_t("blend"));

		SMAABuffer->Initialize();
		SMAABuffer->Bind();
		SMAABuffer->AddAttachment(new frameBuffer::attachment_t("SMAA"));

		for(unsigned int iter = 0; iter < numPreviousFrames; iter++)
		{
			frameBuffer* newBuffer = new frameBuffer();

			newBuffer->Initialize();
			newBuffer->Bind();
			newBuffer->AddAttachment(new frameBuffer::attachment_t(("color" + std::to_string(iter))));
			newBuffer->AddAttachment(new frameBuffer::attachment_t(("depth" + std::to_string(iter)), depthDesc));

			historyFrames.push_back(newBuffer);
		}

		edgeDetectionProgram = shaderPrograms[1]->handle;
		blendingWeightProgram = shaderPrograms[2]->handle;
		SMAAProgram = shaderPrograms[3]->handle;
		resolveProgram = shaderPrograms[4]->handle;
		compareProgram = shaderPrograms[5]->handle;
		finalProgram = shaderPrograms[6]->handle;
		
		frameBuffer::Unbind();
	}

protected:

	frameBuffer* downscaleBuffer;
	unsigned int downscaleProgram = 0;
	bufferHandler_t<downscaleSettings_t> downscaleUniforms;
	bufferHandler_t<lanzcosSettings_t> lanzcosUniforms;

	virtual void Update() override
	{
		SMAA1xt::Update();

		downscaleUniforms.Update();
		lanzcosUniforms.Update();
	}

	virtual void Draw() override
	{
		velocityUniforms.data.currentView = sceneCamera->view;
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		glm::vec2 defaultResolution = sceneCamera->resolution;
		sceneCamera->resolution = glm::vec2(sceneCamera->resolution.x * 2, sceneCamera->resolution.y * 2);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		JitterPass(); //render current scene with jitter
		sceneCamera->resolution = defaultResolution;

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();
		
		EdgeDetectionPass();
		BlendingWeightsPass();

		SMAAPass();

		SMAAResolvePass();

		FinalPass(historyFrames[currentFrame]->attachments[0], geometryBuffer->attachments[0]);
		
		DrawGUI(windows[0]);
		
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void JitterPass()
	{
		geometryBuffer->Bind();

		GLenum drawbuffers[2] = {
			geometryBuffer->attachments[0]->FBODesc.attachmentFormat, //color
			geometryBuffer->attachments[1]->FBODesc.attachmentFormat, //velocity
		};

		glDrawBuffers(2, drawbuffers);

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

		geometryBuffer->attachments[0]->SetActive(0); // color
		weightsBuffer->attachments[0]->SetActive(1); //blending weights

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(SMAAProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		SMAABuffer->Unbind();
	}

	virtual void SMAAResolvePass()
	{
		historyFrames[currentFrame]->Bind();
		GLenum drawBuffers[1] = {
			historyFrames[currentFrame]->attachments[0]->FBODesc.attachmentFormat
		};
		glDrawBuffers(1, drawBuffers);

		SMAABuffer->attachments[0]->SetActive(0); //current color
		geometryBuffer->attachments[2]->SetActive(1);//current depth

		historyFrames[!currentFrame]->attachments[0]->SetActive(2); //previous color
		historyFrames[!currentFrame]->attachments[1]->SetActive(3);//previous depth

		geometryBuffer->attachments[1]->SetActive(4); //velocity
		
		

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(resolveProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		historyFrames[currentFrame]->Unbind();
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
		ImGui::Separator();
		ImGui::DragFloat("halton scale", &jitterUniforms.data.haltonScale, 0.1f, 0.0f, 15.0f, "%.3f");
		ImGui::DragInt("halton index", &jitterUniforms.data.haltonIndex, 1.0f, 0, 128);

		ImGui::End();
	}

	virtual void DrawTAASettings()
	{
		ImGui::Begin("TAA Settings");
		ImGui::Text("performance | %f", defaultTimer->GetTimeMilliseconds());
		ImGui::Checkbox("enable Compare", &enableCompare);
		//ImGui::SliderFloat("feedback factor", &taaUniforms.data.feedbackFactor, 0.0f, 1.0f);
		//ImGui::InputFloat("max depth falloff", &taaUniforms.data.maxDepthFalloff, 0.01f);

		//velocity settings
		ImGui::Separator();
		//ImGui::SameLine();
		//ImGui::Text("Velocity settings");
		//ImGui::SliderFloat("Velocity scale", &taaUniforms.data.velocityScale, 0.0f, 10.0f);

		//jitter settings
		//ImGui::Separator();
		//ImGui::SameLine();
		ImGui::DragFloat("halton scale", &jitterUniforms.data.haltonScale, 0.1f, 0.0f, 15.0f, "%.3f");
		ImGui::DragInt("halton index", &jitterUniforms.data.haltonIndex, 1.0f, 0, 128);
		ImGui::DragInt("enable dithering", &jitterUniforms.data.enableDithering, 1.0f, 0, 1);
		ImGui::DragFloat("dithering scale", &jitterUniforms.data.ditheringScale, 1.0f, 0.0f, 1000.0f, "%.3f");

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

		for (auto iter : historyFrames)
		{
			for (auto iter2 : iter->attachments)
			{
				ImGui::Image((ImTextureID*)iter2->GetHandle(), ImVec2(512, 288),
					ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Text("%s\n", iter2->GetUniformName().c_str());
			}
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
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		historyFrames[!currentFrame]->Bind(); //clear the previous, the next frame current becomes previous
		historyFrames[!currentFrame]->ClearTexture(historyFrames[!currentFrame]->attachments[0], clearColor1);
		historyFrames[!currentFrame]->ClearTexture(historyFrames[!currentFrame]->attachments[1], clearColor2);
		//copy current depth to previous or vice versa?
		historyFrames[currentFrame]->attachments[1]->Copy(geometryBuffer->attachments[2]); //copy depth over

		glClear(GL_DEPTH_BUFFER_BIT);
		historyFrames[currentFrame]->Unbind();

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		geometryBuffer->ClearTexture(geometryBuffer->attachments[1], clearColor2);
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
		velocityUniforms.data.previousProjection = sceneCamera->projection;
		velocityUniforms.data.previousView = sceneCamera->view;
		velocityUniforms.data.prevTranslation = testModel->makeTransform(); //could be jittering the camera instead of the geometry?
		velocityUniforms.Update();
	}

	virtual void ResizeBuffers(glm::ivec2 resolution) override
	{
		SMAA::ResizeBuffers(resolution);
		for (auto frame : historyFrames)
		{
			for (auto iter : frame->attachments)
			{
				iter->Resize(glm::ivec3(resolution, 1));
			}
		}
	}

	virtual void InitializeUniforms() override
	{
		SMAA::InitializeUniforms();

		jitterUniforms.Initialize(4);
		jitter2Uniforms.Initialize(3);
		velocityUniforms.Initialize(1);
		taaUniforms.Initialize(2);
	}
};
#endif