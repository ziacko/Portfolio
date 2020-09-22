#ifndef TXAA_H
#define TXAA_H

#include "Scene3D.h"
#include "FrameBuffer.h"

typedef enum { SMAA = 0, INSIDE, INSIDE2, CUSTOM, EXPERIMENTAL } TAAResolves_t;

struct temporalAAFrame
{
	temporalAAFrame(frameBuffer::attachment_t* color, frameBuffer::attachment_t* depth, frameBuffer::attachment_t* position)
	{
		if (color != nullptr && depth != nullptr && position != nullptr)
		{
			attachments.push_back(color);
			attachments.push_back(depth);
			attachments.push_back(position);
		}
	}

	std::vector<frameBuffer::attachment_t*> attachments;

	//also grab the view, translation and projection
};

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
		haltonScale = 250.0f;
		ditheringScale = 0.0f;
	}

	~jitterSettings_t() {};
};

struct reprojectSettings_t
{
	glm::mat4		previousProjection;
	glm::mat4		previousView;
	glm::mat4		prevTranslation;

	glm::mat4		currentView;

	reprojectSettings_t()
	{
		this->previousProjection = glm::mat4(1.0f);
		this->previousView = glm::mat4(1.0f);
		this->prevTranslation = glm::mat4(1.0f);

		this->currentView = glm::mat4(1.0f);
	}

	~reprojectSettings_t() {};
};

struct TAASettings_t
{
	//velocity
	float velocityScale;
	//Inside
	float feedbackFactor;
	//Custom
	float maxDepthFalloff;

	TAASettings_t()
	{
		this->feedbackFactor = 0.9f;
		this->maxDepthFalloff = 1.0f;
		this->velocityScale = 1.0f;
	}

	~TAASettings_t() { };
};

struct sharpenSettings_t
{
	GLfloat			kernel1;
	GLfloat			kernel2;

	sharpenSettings_t(
		GLfloat kernel1 = -0.125f, GLfloat kernel2 = 1.75f)
	{
		this->kernel1 = kernel1;
		this->kernel2 = kernel2;
	}

	~sharpenSettings_t() { };
};

struct FXAASettings_t
{
	GLfloat			pixelShift;
	GLfloat			vxOffset;
	GLfloat			maxSpan;
	GLfloat			reduceMul;
	GLfloat			reduceMin;

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

class TXAA : public scene3D
{
public:

	TXAA(
		const char* windowName = "Ziyad Barakat's portfolio (TAA + FXAA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/TXAA.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		//glEnable(gl_clip_distance0);
		glDepthFunc(GL_LESS);
		glHint(gl_generate_mipmap_hint, GL_NICEST);

		geometryBuffer = new frameBuffer();
		unJitteredBuffer = new frameBuffer();
		FXAABuffer = new frameBuffer();
		sharpenBuffer = new frameBuffer();
		

		velocityUniforms = bufferHandler_t<reprojectSettings_t>();
		taaUniforms = bufferHandler_t<TAASettings_t>();
		FXAASettings = bufferHandler_t<FXAASettings_t>();
		sharpenSettings = bufferHandler_t<sharpenSettings_t>();
		jitterUniforms = bufferHandler_t<jitterSettings_t>();
		this->sharpenSettings = bufferHandler_t<sharpenSettings_t>();

		for (int iter = 0; iter < 128; iter++)
		{
			jitterUniforms.data.haltonSequence[iter] = glm::vec2(CreateHaltonSequence(iter + 1, 2), CreateHaltonSequence(iter + 1, 3));
		}
		//glGenQueries(1, &defaultQuery);
		//glGenQueries(1, &TAAQuery);
	}

	~TXAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		//glGenQueries(1, &jitterQuery);

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		FBODescriptor velDesc;
		velDesc.format = gl_rg;
		velDesc.internalFormat = gl_rg16_snorm;

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("velocity",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height),
			velDesc));

		FBODescriptor depthDesc;
		depthDesc.dataType = GL_FLOAT;
		depthDesc.format = GL_DEPTH_COMPONENT;
		depthDesc.internalFormat = gl_depth_component24;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depth;

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height),
			depthDesc));

		for (unsigned int iter = 0; iter < numPrevFrames; iter++)
		{
			frameBuffer* newBuffer = new frameBuffer();

			newBuffer->Initialize();
			newBuffer->Bind();
			newBuffer->AddAttachment(new frameBuffer::attachment_t(("color" + std::to_string(iter)),
				glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

			newBuffer->AddAttachment(new frameBuffer::attachment_t(("depth" + std::to_string(iter)), glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height),
				depthDesc));

			TAAFrames.push_back(newBuffer);
		}

		/*sharpenBuffer->Bind();
		sharpenBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"sharp", glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));*/

		unJitteredBuffer->Initialize();
		unJitteredBuffer->Bind();
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t("unJittered",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t("depth",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height),
			depthDesc));

		sharpenBuffer->Initialize();
		sharpenBuffer->Bind();
		sharpenBuffer->AddAttachment(new frameBuffer::attachment_t("Sharpen",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		FXAABuffer->Initialize();
		FXAABuffer->Bind();
		FXAABuffer->AddAttachment(new frameBuffer::attachment_t("FXAA",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));


		//geometry automatically gets assigned to 0
		unjitteredProgram = shaderPrograms[1]->handle;
		FXAAProgram = shaderPrograms[2]->handle;
		smoothProgram = shaderPrograms[3]->handle;
		
		sharpenProgram = shaderPrograms[4]->handle;
		compareProgram = shaderPrograms[5]->handle;
		finalProgram = shaderPrograms[6]->handle;

		//averageGPUTimes.reserve(10);

		frameBuffer::Unbind();
	}

protected:

	std::vector<frameBuffer*> TAAFrames;
	frameBuffer* geometryBuffer;
	frameBuffer* unJitteredBuffer;
	frameBuffer* FXAABuffer;
	frameBuffer* sharpenBuffer;

	unsigned int unjitteredProgram = 0;
	unsigned int FXAAProgram = 0;
	unsigned int smoothProgram = 0;
	
	unsigned int sharpenProgram = 0;
	unsigned int compareProgram = 0;
	unsigned int finalProgram = 0;

	unsigned int jitterQuery = 0;
	unsigned int defaultQuery = 0;
	unsigned int TAAQuery = 0;

	GLuint numPrevFrames = 2; //don't need this right now

	bufferHandler_t<reprojectSettings_t>	velocityUniforms;

	bufferHandler_t<TAASettings_t>			taaUniforms;
	bufferHandler_t<FXAASettings_t>			FXAASettings;

	std::vector<const char*>		TAAResolveSettings = { "SMAA", "Inside", "Inside2", "Custom", "Experimental" };
	bool enableCompare = true;

	bufferHandler_t<sharpenSettings_t>			sharpenSettings;

	bufferHandler_t<jitterSettings_t> jitterUniforms;

	bool currentFrame = 0;

	std::vector<uint64_t> averageGPUTimes;

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

		taaUniforms.Update();
		sharpenSettings.Update();
		jitterUniforms.Update();
		FXAASettings.Update();
	
		currentFrame = ((defaultPayload.data.totalFrames % 2) == 0) ? 0 : 1;//if even frame then write to 1 and read from 0 and vice versa
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
		velocityUniforms.data.currentView = sceneCamera->view; //set to the previous view matrix?
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		JitterPass(); //render current scene with jitter

		if (enableCompare)
		{
			UnJitteredPass();
		}

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();
		
		FXAAPass();

		TAAPass(); //use the positions, colors, depth and velocity to smooth the final image

		SharpenPass();

		FinalPass(sharpenBuffer->attachments[0], unJitteredBuffer->attachments[0]);
		
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
			geometryBuffer->attachments[1]->FBODesc.attachmentFormat //velocity
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
			//add the previous depth?

			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(this->programGLID);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
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

	virtual void TAAPass()
	{
		TAAFrames[currentFrame]->Bind();
		GLenum drawBuffers[1] = {
			TAAFrames[currentFrame]->attachments[0]->FBODesc.attachmentFormat
		};
		glDrawBuffers(1, drawBuffers);

		//current frame
		FXAABuffer->attachments[0]->SetActive(0); //FXAA color
		geometryBuffer->attachments[2]->SetActive(1); //current depth

		//previous frames
		TAAFrames[!currentFrame]->attachments[0]->SetActive(2); //previous color
		TAAFrames[!currentFrame]->attachments[1]->SetActive(3); //previous depth

		geometryBuffer->attachments[1]->SetActive(4); //velocity
		
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(smoothProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		TAAFrames[currentFrame]->Unbind();
	}

	virtual void FXAAPass()
	{
		FXAABuffer->Bind();
		GLenum drawBuffers[1] = {
			FXAABuffer->attachments[0]->FBODesc.attachmentFormat
		};
		glDrawBuffers(1, drawBuffers);

		//current frame
		geometryBuffer->attachments[0]->SetActive(0); // color

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(FXAAProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		FXAABuffer->Unbind();
	}

	virtual void SharpenPass()
	{
		sharpenBuffer->Bind();
		GLenum drawBuffers[1] = {
			sharpenBuffer->attachments[0]->FBODesc.attachmentFormat
		};
		glDrawBuffers(1, drawBuffers);

		//current frame
		TAAFrames[currentFrame]->attachments[0]->SetActive(0); // color

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(sharpenProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		sharpenBuffer->Unbind();
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
		DrawFXAASettings();
		DrawSharpenSettings();
		//DrawJitterSettings();
	}

	void DrawFXAASettings()
	{
		ImGui::Begin("FXAA Settings");
		ImGui::Checkbox("enable Compare", &enableCompare);
		ImGui::SliderFloat("Sub pixel drift", &FXAASettings.data.pixelShift, 0.0f, 1.0f, "%.1f");
		ImGui::SliderFloat("vertex Offset", &FXAASettings.data.vxOffset, 0.0f, 1.0f, "%.3f");
		ImGui::SliderFloat("max span", &FXAASettings.data.maxSpan, 0.0f, 10.0f, "%.1f");
		ImGui::SliderFloat("reduce multiplier", &FXAASettings.data.reduceMul, 0.0f, 1.0f, "%.5f");
		ImGui::SliderFloat("reduce minimizer", &FXAASettings.data.reduceMin, 0.0f, 1.0f, "%.8f");

		ImGui::End();
	}

	virtual void DrawSharpenSettings()
	{
		ImGui::Begin("Sharpen settings");

		ImGui::SliderFloat("kernel 1", &sharpenSettings.data.kernel1, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 5", &sharpenSettings.data.kernel2, 0.0f, 10.0f, "%.5f", 1.0f);		

		ImGui::End();
	}

	virtual void DrawTAASettings()
	{
		ImGui::Begin("TAA Settings");
		ImGui::Checkbox("enable Compare", &enableCompare);
		ImGui::SliderFloat("feedback factor", &taaUniforms.data.feedbackFactor, 0.0f, 1.0f);
		ImGui::InputFloat("max depth falloff", &taaUniforms.data.maxDepthFalloff, 0.01f);

		//velocity settings
		ImGui::Separator();
		ImGui::SameLine();
		ImGui::Text("Velocity settings");
		ImGui::SliderFloat("Velocity scale", &taaUniforms.data.velocityScale, 0.0f, 10.0f);

		//jitter settings
		ImGui::Separator();
		//ImGui::SameLine();
		ImGui::DragFloat("halton scale", &jitterUniforms.data.haltonScale, 0.1f, 0.0f, 15.0f, "%.3f");
		ImGui::DragInt("halton index",  &jitterUniforms.data.haltonIndex, 1.0f, 0, 128);
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

		ImGui::Image((ImTextureID*)FXAABuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", FXAABuffer->attachments[0]->GetUniformName().c_str());

		for (auto iter : TAAFrames)
		{
			for (auto iter2 : iter->attachments)
			{
				ImGui::Image((ImTextureID*)iter2->GetHandle(), ImVec2(512, 288),
					ImVec2(0, 1), ImVec2(1, 0));
				ImGui::SameLine();
				ImGui::Text("%s\n", iter2->GetUniformName().c_str());
			}
		}

		ImGui::Image((ImTextureID*)unJitteredBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", unJitteredBuffer->attachments[0]->GetUniformName().c_str());

		ImGui::Image((ImTextureID*)sharpenBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", sharpenBuffer->attachments[0]->GetUniformName().c_str());

		ImGui::End();
	}

	virtual void DrawJitterSettings()
	{
		ImGui::Begin("Jitter Settings");


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

		TAAFrames[!currentFrame]->Bind(); //clear the previous, the next frame current becomes previous
		TAAFrames[!currentFrame]->ClearTexture(TAAFrames[!currentFrame]->attachments[0], clearColor1);
		TAAFrames[!currentFrame]->ClearTexture(TAAFrames[!currentFrame]->attachments[1], clearColor2);
		//copy current depth to previous or vice versa?
		TAAFrames[currentFrame]->attachments[1]->Copy(geometryBuffer->attachments[2]); //copy depth over

		glClear(GL_DEPTH_BUFFER_BIT);
		TAAFrames[currentFrame]->Unbind();

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		geometryBuffer->ClearTexture(geometryBuffer->attachments[1], clearColor2);
		geometryBuffer->ClearTexture(geometryBuffer->attachments[2], clearColor2);
		glClear(GL_DEPTH_BUFFER_BIT);
		geometryBuffer->Unbind();

		unJitteredBuffer->Bind();
		unJitteredBuffer->ClearTexture(unJitteredBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		unJitteredBuffer->Unbind();

		FXAABuffer->Bind();
		FXAABuffer->ClearTexture(FXAABuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		FXAABuffer->Unbind();

		sharpenBuffer->Bind();
		sharpenBuffer->ClearTexture(sharpenBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		sharpenBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		velocityUniforms.data.previousProjection = sceneCamera->projection;
		velocityUniforms.data.previousView = sceneCamera->view;
		velocityUniforms.data.prevTranslation = testModel->makeTransform();
		velocityUniforms.Update();
	}

	virtual void ResizeBuffers(glm::vec2 resolution)
	{
		for (auto frame : TAAFrames)
		{
			for (auto iter : frame->attachments)
			{
				iter->Resize(resolution);
			}
		}

		FXAABuffer->attachments[0]->Resize(resolution);

		for (auto iter : geometryBuffer->attachments)
		{
			iter->Resize(resolution);
		}		

		//sharpenBuffer->attachments[0]->Resize(resolution);
		unJitteredBuffer->attachments[0]->Resize(resolution);
		unJitteredBuffer->attachments[1]->Resize(resolution);

		sharpenBuffer->attachments[0]->Resize(resolution);
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
		velocityUniforms.Initialize(1);
		taaUniforms.Initialize(2);
		sharpenSettings.Initialize(3);
		jitterUniforms.Initialize(4);
		FXAASettings.Initialize(5);

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