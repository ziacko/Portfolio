#ifndef TEMPORALAA_H
#define TEMPORALAA_H


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

struct reprojectSettings_t
{
	glm::mat4		previousProjection;
	glm::mat4		previousView;
	glm::mat4		prevTranslation;

	glm::mat4		currentView;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	reprojectSettings_t()
	{
		this->previousProjection = glm::mat4(1.0f);
		this->previousView = glm::mat4(1.0f);
		this->prevTranslation = glm::mat4(1.0f);

		this->currentView = glm::mat4(1.0f);

		bufferHandle = 0;
		uniformHandle = 0;
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

	GLuint			bufferHandle;
	GLuint			uniformHandle;

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

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	sharpenSettings_t(
		GLfloat kernel1 = -0.125f, GLfloat kernel2 = 1.75f)
	{
		this->kernel1 = kernel1;
		this->kernel2 = kernel2;
	}

	~sharpenSettings_t() { };
};

struct jitterSettings_t
{
	glm::vec2			haltonSequence[128];
	float				haltonScale;
	int					haltonIndex;
	int					enableDithering;
	float				ditheringScale;

	GLuint				bufferHandle;
	GLuint				uniformHandle;

	jitterSettings_t()
	{
		haltonIndex = 16;
		enableDithering = 1;
		haltonScale = 1.0f;
		ditheringScale = 0.0f;
	}

	~jitterSettings_t() {};
};

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
		

		velocityUniforms = new reprojectSettings_t();
		taaUniforms = new TAASettings_t();
		FXAASettings = new FXAASettings_t();
		sharpenSettings = new sharpenSettings_t();
		jitterUniforms = new jitterSettings_t();

		for (int iter = 0; iter < 128; iter++)
		{
			jitterUniforms->haltonSequence[iter] = glm::vec2(CreateHaltonSequence(iter + 1, 2), CreateHaltonSequence(iter + 1, 3));
		}
		//glGenQueries(1, &defaultQuery);
		//glGenQueries(1, &TAAQuery);
	}

	~TXAA() {};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		glGenQueries(1, &jitterQuery);

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"color", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"velocity", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height),
			gl_rg, GL_TEXTURE_2D, GL_FLOAT, gl_rg16_snorm));

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::depth,
			"depth", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height),
			GL_DEPTH_COMPONENT, GL_TEXTURE_2D, GL_FLOAT, gl_depth_component24));

		FXAABuffer->Initialize();
		FXAABuffer->Bind();
		FXAABuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"FXAA", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));

		for (unsigned int iter = 0; iter < numPrevFrames; iter++)
		{
			frameBuffer* newBuffer = new frameBuffer();

			newBuffer->Initialize();
			newBuffer->Bind();
			newBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
				("color" + std::to_string(iter)), glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));

			newBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::depth,
				("depth" + std::to_string(iter)), glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height),
				GL_DEPTH_COMPONENT, GL_TEXTURE_2D, GL_FLOAT, gl_depth_component24));

			TAAFrames.push_back(newBuffer);
		}

		/*sharpenBuffer->Bind();
		sharpenBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"sharp", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));*/

		unJitteredBuffer->Initialize();
		unJitteredBuffer->Bind();
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
			"unJittered", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));
		unJitteredBuffer->AddAttachment(new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::depth,
			"depth", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height),
			GL_DEPTH_COMPONENT, GL_TEXTURE_2D, GL_FLOAT, gl_depth_component24));

		//geometry automatically gets assigned to 0
		unjitteredProgram = shaderPrograms[1]->handle;
		FXAAProgram = shaderPrograms[2]->handle;
		smoothProgram = shaderPrograms[3]->handle;
		
		//sharpenProgram = shaderPrograms[2]->handle;
		compareProgram = shaderPrograms[4]->handle;
		finalProgram = shaderPrograms[5]->handle;

		averageGPUTimes.reserve(10);

		frameBuffer::Unbind();
	}

protected:

	std::vector<frameBuffer*> TAAFrames;
	frameBuffer* geometryBuffer;
	frameBuffer* unJitteredBuffer;
	frameBuffer* FXAABuffer;
	//frameBuffer* sharpenBuffer;

	unsigned int unjitteredProgram = 0;
	unsigned int FXAAProgram = 0;
	unsigned int smoothProgram = 0;
	
	//unsigned int sharpenProgram = 0;
	unsigned int compareProgram = 0;
	unsigned int finalProgram = 0;

	unsigned int jitterQuery = 0;
	unsigned int defaultQuery = 0;
	unsigned int TAAQuery = 0;

	GLuint numPrevFrames = 2; //don't need this right now

	reprojectSettings_t* velocityUniforms;

	TAASettings_t*			taaUniforms;
	FXAASettings_t*			FXAASettings;

	std::vector<const char*>		TAAResolveSettings = { "SMAA", "Inside", "Inside2", "Custom", "Experimental" };
	bool enableCompare = true;

	sharpenSettings_t*			sharpenSettings;
	bool enableSharpen = false;

	jitterSettings_t* jitterUniforms;

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

		defaultUniform->deltaTime = (float)sceneClock->GetDeltaTime();
		defaultUniform->totalTime = (float)sceneClock->GetTotalTime();
		defaultUniform->framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultUniform->totalFrames++;

		UpdateBuffer(taaUniforms, taaUniforms->bufferHandle, sizeof(*taaUniforms), gl_uniform_buffer, gl_dynamic_draw);
		UpdateBuffer(sharpenSettings, sharpenSettings->bufferHandle, sizeof(*sharpenSettings), gl_uniform_buffer, gl_dynamic_draw);
		UpdateBuffer(jitterUniforms, jitterUniforms->bufferHandle, sizeof(*jitterUniforms), gl_uniform_buffer, gl_dynamic_draw);
		UpdateBuffer(FXAASettings, FXAASettings->bufferHandle, sizeof(*FXAASettings), gl_uniform_buffer, gl_dynamic_draw);
	
		currentFrame = ((defaultUniform->totalFrames % 2) == 0) ? 0 : 1;//if even frame then write to 1 and read from 0 and vice versa
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
		velocityUniforms->currentView = sceneCamera->view; //set to the previous view matrix?
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

		//SharpenPass();

		FinalPass(TAAFrames[currentFrame]->attachments[0], unJitteredBuffer->attachments[0]);
		
		DrawGUI(windows[0]);
		
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void JitterPass()
	{
		geometryBuffer->Bind();

		GLenum drawbuffers[2] = {
			geometryBuffer->attachments[0]->attachmentFormat, //color
			geometryBuffer->attachments[1]->attachmentFormat //velocity
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
			glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
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
			unJitteredBuffer->attachments[0]->attachmentFormat, //color
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
			glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
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
			TAAFrames[currentFrame]->attachments[0]->attachmentFormat
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
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		TAAFrames[currentFrame]->Unbind();
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
		DrawTAASettings();
		DrawFXAASettings();
		//DrawSharpenSettings();
		//DrawJitterSettings();
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

	virtual void DrawSharpenSettings()
	{
		ImGui::Begin("Sharpen settings");
		ImGui::Checkbox("enable sharpen", &enableSharpen);

		ImGui::SliderFloat("kernel 1", &sharpenSettings->kernel1, -1.0f, 1.0f);
		ImGui::SliderFloat("kernel 5", &sharpenSettings->kernel2, 0.0f, 10.0f, "%.5f", 1.0f);		

		ImGui::End();
	}

	virtual void DrawTAASettings()
	{
		ImGui::Begin("TAA Settings");
		ImGui::Checkbox("enable Compare", &enableCompare);
		ImGui::SliderFloat("feedback factor", &taaUniforms->feedbackFactor, 0.0f, 1.0f);
		ImGui::InputFloat("max depth falloff", &taaUniforms->maxDepthFalloff, 0.01f);

		//velocity settings
		ImGui::Separator();
		ImGui::SameLine();
		ImGui::Text("Velocity settings");
		ImGui::SliderFloat("Velocity scale", &taaUniforms->velocityScale, 0.0f, 10.0f);

		//jitter settings
		ImGui::Separator();
		//ImGui::SameLine();
		ImGui::DragFloat("halton scale", &jitterUniforms->haltonScale, 0.1f, 0.0f, 15.0f, "%.3f");
		ImGui::DragInt("halton index",  &jitterUniforms->haltonIndex, 1.0f, 0, 128);
		ImGui::DragInt("enable dithering", &jitterUniforms->enableDithering, 1.0f, 0, 1);
		ImGui::DragFloat("dithering scale", &jitterUniforms->ditheringScale, 1.0f, 0.0f, 1000.0f, "%.3f");

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

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		velocityUniforms->previousProjection = sceneCamera->projection;
		velocityUniforms->previousView = sceneCamera->view;
		velocityUniforms->prevTranslation = testModel->makeTransform();
		UpdateBuffer(velocityUniforms, velocityUniforms->bufferHandle, sizeof(*velocityUniforms), gl_uniform_buffer, gl_dynamic_draw);
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
		SetupBuffer(velocityUniforms, velocityUniforms->bufferHandle, sizeof(*velocityUniforms), 1, gl_uniform_buffer, gl_dynamic_draw);
		SetupBuffer(taaUniforms, taaUniforms->bufferHandle, sizeof(*taaUniforms), 2, gl_uniform_buffer, gl_dynamic_draw);
		SetupBuffer(sharpenSettings, sharpenSettings->bufferHandle, sizeof(*sharpenSettings), 3, gl_uniform_buffer, gl_dynamic_draw);
		SetupBuffer(jitterUniforms, jitterUniforms->bufferHandle, sizeof(*jitterUniforms), 4, gl_uniform_buffer, gl_dynamic_draw);
		SetupBuffer(FXAASettings, FXAASettings->bufferHandle, sizeof(*FXAASettings), 5, gl_uniform_buffer, gl_dynamic_draw);
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