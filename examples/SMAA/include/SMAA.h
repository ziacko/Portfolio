#ifndef TEMPORALAA_H
#define TEMPORALAA_H


#include "Scene3D.h"
#include "FrameBuffer.h"

struct FXAASettings_t
{
	GLfloat			pixelShift;
	GLfloat			vxOffset;
	GLfloat			maxSpan;
	GLfloat			reduceMul;
	GLfloat			reduceMin;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	FXAASettings_t(
		GLfloat pixelShift = 0.25f, GLfloat vxOffset = 0.0f, GLfloat maxSpan = 8.0f, 
		GLfloat reduceMul = 0.125f, GLfloat reduceMin = 0.0078125f)
	{
		this->pixelShift = pixelShift;
		this->vxOffset = vxOffset;
		this->maxSpan = maxSpan;
		this->reduceMul = reduceMul;
		this->reduceMin = reduceMin;
	}

	~FXAASettings_t() { };
};

class FXAA : public scene3D
{
public:

	FXAA(
		const char* windowName = "Ziyad Barakat's portfolio (FXAA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/FXAA.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glHint(gl_generate_mipmap_hint, GL_NICEST);

		geometryBuffer = new frameBuffer();
		FXAABuffer = new frameBuffer();

		FXAASettings = new FXAASettings_t();
	}

	~FXAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"color", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::depth,
			"depth", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height), texture::textureType_t::image,
			GL_DEPTH_COMPONENT, GL_TEXTURE_2D, 0, 0, 0, GL_FLOAT, gl_depth_component24));

		FXAABuffer->Initialize();
		FXAABuffer->Bind();
		FXAABuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"color", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));

		FXAAProgram = shaderPrograms[1]->handle;
		compareProgram = shaderPrograms[2]->handle;
		finalProgram = shaderPrograms[3]->handle;
		
		frameBuffer::Unbind();
	}

protected:

	frameBuffer* geometryBuffer;
	frameBuffer* FXAABuffer;

	unsigned int FXAAProgram = 0;
	unsigned int compareProgram = 0;
	unsigned int finalProgram = 0;

	bool enableCompare = true;

	FXAASettings_t*			FXAASettings;

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

		UpdateBuffer(FXAASettings, FXAASettings->bufferHandle, sizeof(*FXAASettings), gl_uniform_buffer, gl_dynamic_draw);
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
		
		FXAAPass(); //use the positions, colors, depth and velocity to smooth the final image

		FinalPass(FXAABuffer->attachments[0], geometryBuffer->attachments[0]);
		
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

	virtual void FXAAPass()
	{
		FXAABuffer->Bind();
		GLenum drawBuffers[1] = {
			FXAABuffer->attachments[0]->attachmentFormat
		};
		glDrawBuffers(1, drawBuffers);

		//current frame
		geometryBuffer->attachments[0]->SetActive(0); // color
		
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(FXAAProgram);
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		FXAABuffer->Unbind();
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

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(windows[0], io);

		DrawBufferAttachments();
		DrawFXAASettings();
	}

	void DrawFXAASettings()
	{
		ImGui::Begin("FXAA Settings");
		ImGui::Checkbox("enable Compare", &enableCompare);
		ImGui::SliderFloat("Sub pixel drift", &FXAASettings->pixelShift, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("vertex Offset", &FXAASettings->vxOffset, 0.0f, 1.0f, "%.3f");
		ImGui::SliderFloat("max span", &FXAASettings->maxSpan, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("reduce multiplier", &FXAASettings->reduceMul, 0.0f, 1.0f, "%.5f");
		ImGui::SliderFloat("reduce minimizer", &FXAASettings->reduceMin, 0.0f, 1.0f, "%.8f");

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

		for (auto iter : FXAABuffer->attachments)
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

		FXAABuffer->Bind();
		FXAABuffer->ClearTexture(FXAABuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		FXAABuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::vec2 resolution)
	{
		for (auto iter : geometryBuffer->attachments)
		{
			iter->Resize(resolution);
		}

		FXAABuffer->attachments[0]->Resize(resolution);
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
		SetupBuffer(FXAASettings, FXAASettings->bufferHandle, sizeof(*FXAASettings), 1, gl_uniform_buffer, gl_dynamic_draw);
		SetupVertexBuffer();
	}
};
#endif