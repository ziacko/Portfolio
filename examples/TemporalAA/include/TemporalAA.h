#ifndef TEMPORALAA_H
#define TEMPORALAA_H

#include "Scene3D.h"
#include "FrameBuffer.h"
#include "HaltonSequence.h"

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
		haltonScale = 100.0f;
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

class temporalAA : public scene3D
{
public:

	temporalAA(
		const char* windowName = "Ziyad Barakat's portfolio (temporal AA)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective, 0.1f, 2000.f),
		const char* shaderConfigPath = "../../resources/shaders/TemporalAA.txt",
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
		//sharpenBuffer = new frameBuffer();

		velocityUniforms = bufferHandler_t<reprojectSettings_t>();
		taaUniforms = bufferHandler_t<TAASettings_t>();
		sharpenSettings = bufferHandler_t<sharpenSettings_t>();
		jitterUniforms = bufferHandler_t<jitterSettings_t>();

		for (int iter = 0; iter < 128; iter++)
		{
			jitterUniforms.data.haltonSequence[iter] = glm::vec2(CreateHaltonSequence(iter + 1, 2), CreateHaltonSequence(iter + 1, 3));
		}
		//glGenQueries(1, &defaultQuery);
		//glGenQueries(1, &TAAQuery);
	}

	virtual ~temporalAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		FBODescriptor colorDesc;
		colorDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		glGenQueries(1, &jitterQuery);

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color", colorDesc));

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

		for (unsigned int iter = 0; iter < numPrevFrames; iter++)
		{
			frameBuffer* newBuffer = new frameBuffer();

			newBuffer->Initialize();
			newBuffer->Bind();
			newBuffer->AddAttachment(new frameBuffer::attachment_t("color" + std::to_string(iter), colorDesc));
			newBuffer->AddAttachment(new frameBuffer::attachment_t("depth" + std::to_string(iter), depthDesc));

			historyFrames.push_back(newBuffer);
		}

		/*sharpenBuffer->Bind();
		sharpenBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"sharp", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));*/

		unJitteredBuffer->Initialize();
		unJitteredBuffer->Bind();
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t("unJittered", colorDesc));
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t("depth", depthDesc));

		//geometry automatically gets assigned to 0
		smoothProgram = shaderPrograms[1]->handle;
		unjitteredProgram = shaderPrograms[2]->handle;
		//sharpenProgram = shaderPrograms[2]->handle;
		compareProgram = shaderPrograms[3]->handle;
		finalProgram = shaderPrograms[4]->handle;

		averageGPUTimes.reserve(10);

		frameBuffer::Unbind();
	}

protected:

	std::vector<frameBuffer*> historyFrames;
	frameBuffer* geometryBuffer;
	frameBuffer* unJitteredBuffer;
	//frameBuffer* sharpenBuffer;

	unsigned int smoothProgram = 0;
	unsigned int unjitteredProgram = 0;
	//unsigned int sharpenProgram = 0;
	unsigned int compareProgram = 0;
	unsigned int finalProgram = 0;

	unsigned int jitterQuery = 0;
	unsigned int defaultQuery = 0;
	unsigned int TAAQuery = 0;

	GLuint numPrevFrames = 2; //don't need this right now

	bufferHandler_t<reprojectSettings_t>	velocityUniforms;

	bufferHandler_t<TAASettings_t>			taaUniforms;

	std::vector<const char*>				TAAResolveSettings = { "SMAA", "Inside", "Inside2", "Custom", "Experimental" };
	bool enableCompare = true;

	bufferHandler_t<sharpenSettings_t>		sharpenSettings;
	bool enableSharpen = false;

	bufferHandler_t<jitterSettings_t>		jitterUniforms;

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
		velocityUniforms.data.currentView = sceneCamera->view; //need to update the current view matrix
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		//glBeginQuery(gl_time_elapsed, jitterQuery);

		defaultTimer->Begin();
		JitterPass(); //render current scene with jitter
		defaultTimer->End();

		//glEndQuery(gl_time_elapsed);

		/*glQueryCounter(jitterQuery, gl_timestamp);
		GLint64 geomTime = 0;
		glGetInteger64v(gl_timestamp, &geomTime);
		uint64_t GeomTimeU = static_cast<uint64_t>(geomTime);

		uint64_t averageGPUTime = 0;
		if ((defaultPayload.data.totalFrames % 11) == 0)
		{
			//average the whole lot and clear the vector
			uint64_t tempTime = 0;
			for (auto iter : averageGPUTimes)
			{
				tempTime += iter;
			}

			tempTime /= 10; //wee need the average GPU time over 10 frames

			//printf("%f | %f \n", (float)tempTime / 10000.0f, (float)(1.0f / (tempTime / 10000000.0f)));
			averageGPUTimes.clear();
		}

		else
		{
			//
			averageGPUTimes.push_back(GeomTimeU - startTime);
		}*/

		if (enableCompare)
		{
			UnJitteredPass();
		}

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();
		
		TAAPass(); //use the positions, colors, depth and velocity to smooth the final image

		//SharpenPass();

		FinalPass(historyFrames[currentFrame]->attachments[0], unJitteredBuffer->attachments[0]);
		
		DrawGUI(windows[0]);
		
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void JitterPass()
	{
		geometryBuffer->Bind();
		geometryBuffer->DrawAll();

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
		unJitteredBuffer->DrawAll();

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
		historyFrames[currentFrame]->Bind();
		historyFrames[currentFrame]->attachments[0]->Draw();

		//current frame
		geometryBuffer->attachments[0]->SetActive(0); //current color
		geometryBuffer->attachments[2]->SetActive(1); //current depth

		//previous frames
		historyFrames[!currentFrame]->attachments[0]->SetActive(2); //previous color
		historyFrames[!currentFrame]->attachments[1]->SetActive(3); //previous depth

		geometryBuffer->attachments[1]->SetActive(4); //velocity
		
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(smoothProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		historyFrames[currentFrame]->Unbind();
	}

	virtual void FinalPass(texture* tex1, texture* tex2)
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
		//DrawSharpenSettings();
		//DrawJitterSettings();
	}

	virtual void DrawSharpenSettings()
	{
		ImGui::Begin("Sharpen settings");
		ImGui::Checkbox("enable sharpen", &enableSharpen);

		ImGui::SliderFloat("kernel 1", &sharpenSettings.data.kernel1, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 5", &sharpenSettings.data.kernel2, 0.0f, 10.0f, "%.5f", 1.0f);		

		ImGui::End();
	}

	virtual void DrawTAASettings()
	{
		ImGui::Begin("TAA Settings");
		ImGui::Text("performance | %f", defaultTimer->GetTimeMilliseconds());
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

		ImGui::Image((ImTextureID*)unJitteredBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Text("%s\n", unJitteredBuffer->attachments[0]->GetUniformName().c_str());

		ImGui::End();
	}

	virtual void DrawJitterSettings()
	{
		ImGui::Begin("Jitter Settings");
		

		ImGui::End();
	}

	virtual void ClearBuffers()
	{
		//ok copy the current frame into the previous frame and clear the rest of the buffers	
		float clearColor1[4] = { 0.05f, 0.05f, 0.05f, 0.0f };
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		float clearColor3[4] = { 1.0f, 0.0f, 0.0f, 0.0f }; //this is for debugging only!

		historyFrames[!currentFrame]->Bind(); //clear the previous, the next frame current becomes previous
		historyFrames[!currentFrame]->ClearTexture(historyFrames[!currentFrame]->attachments[0], clearColor1);
		historyFrames[!currentFrame]->ClearTexture(historyFrames[!currentFrame]->attachments[1], clearColor1);
		//copy current depth to previous or vice versa?
		historyFrames[currentFrame]->attachments[1]->Copy(geometryBuffer->attachments[2]); //copy depth over

		glClear(GL_DEPTH_BUFFER_BIT);
		historyFrames[currentFrame]->Unbind();

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

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		velocityUniforms.data.previousProjection = sceneCamera->projection;
		velocityUniforms.data.previousView = sceneCamera->view;
		velocityUniforms.data.prevTranslation = testModel->makeTransform(); //could be jittering the camera instead of the geometry?
		velocityUniforms.Update();
	}

	virtual void ResizeBuffers(glm::ivec2 resolution)
	{
		for (auto frame : historyFrames)
		{
			for (auto iter : frame->attachments)
			{
				iter->Resize(glm::ivec3(resolution, 1));
			}
		}

		for (auto iter : geometryBuffer->attachments)
		{
			iter->Resize(glm::ivec3(resolution, 1));
		}

		//sharpenBuffer->attachments[0]->Resize(resolution);
		unJitteredBuffer->attachments[0]->Resize(glm::ivec3(resolution, 1));
		unJitteredBuffer->attachments[1]->Resize(glm::ivec3(resolution, 1));
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

		SetupVertexBuffer();
	}
};

#endif