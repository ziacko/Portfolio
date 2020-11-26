#ifndef HEAT_HAZE_H
#define HEAT_HAZE_H
#include "bubble.h"
#include "FrameBuffer.h"

struct perlinSettings_t//	: public uniformBuffer_t
{
	glm::vec2		uvOffset;
	glm::vec2		uvScale;
	float			modValue;
	float			permuteValue;
	float			taylorInverse;
	float			fadeValue1;
	float			fadeValue2;
	float			fadeValue3;
	int				numOctaves;

	float			noiseValue;
	float			noiseValue2;
	int				colorBias;//20

	float			patternValue1;//0.3
	float			patternValue2;//0.8
	float			patternValue3;//5.2
	float			patternValue4;//1.3
	float			patternValue5;//4.0
	float			patternValue6;//1.7
	float			patternValue7;//9.2
	float			patternValue8;//4.0
	float			patternValue9;//8.3
	float			patternValue10;//2.8
	float			patternValue11;//4.0


	perlinSettings_t(GLfloat modValue = 289.0f, GLfloat permuteValue = 29.33f,
		GLfloat taylorInverse = 1.79284291400159f, GLfloat fadeValue1 = 6.0f, GLfloat fadeValue2 = 15.0f,
		GLfloat fadeValue3 = 10.0f, GLuint numOctaves = 20,
		GLuint colorBias = 2,
		GLfloat noiseValue = 79.66f, GLfloat noiseValue2 = 5.6f,

		GLfloat patternValue1 = 2.5f, GLfloat patternValue2 = 0.4f, GLfloat patternValue3 = 5.2f,
		GLfloat patternValue4 = 1.3f, GLfloat patternValue5 = 4.0f, GLfloat patternValue6 = 1.7f,
		GLfloat patternValue7 = 9.2f, GLfloat patternValue8 = 4.0f, GLfloat patternValue9 = 8.3f,
		GLfloat patternValue10 = 2.8f, GLfloat	patternValue11 = 2.02f)
	{
		this->modValue = modValue;
		this->permuteValue = permuteValue;
		this->taylorInverse = taylorInverse;
		this->fadeValue1 = fadeValue1;
		this->fadeValue2 = fadeValue2;
		this->fadeValue3 = fadeValue3;
		this->numOctaves = numOctaves;

		this->colorBias = colorBias;

		this->noiseValue = noiseValue;
		this->noiseValue2 = noiseValue2;

		this->patternValue1 = patternValue1;
		this->patternValue2 = patternValue2;
		this->patternValue3 = patternValue3;
		this->patternValue4 = patternValue4;
		this->patternValue5 = patternValue5;
		this->patternValue6 = patternValue6;
		this->patternValue7 = patternValue7;
		this->patternValue8 = patternValue8;
		this->patternValue9 = patternValue9;
		this->patternValue10 = patternValue10;
		this->patternValue11 = patternValue11;

		uvOffset = glm::vec2(0);
		uvScale = glm::vec2(1);
	};

	~perlinSettings_t() {};
};

class heatHazeScene : public bubbleScene
{
public:

	heatHazeScene(
		bufferHandler_t<bubbleSettings_t> bubbleSettings = bufferHandler_t<bubbleSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio ( heat haze )",		
		camera* bubbleCamera = new camera(), 
		const char* shaderConfigPath = "../../resources/shaders/HeatHaze.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f) : bubbleScene(bubbleSettings, defaultTexture, windowName, bubbleCamera, shaderConfigPath)
	{

		//this->bubble = bubbleSettings;
		//enableWireframe = true;
		perlinBuffer = new frameBuffer();
		enableWireframe = false;
	}

	~heatHazeScene( void ){}

	virtual void Initialize() override
	{
		perlinBuffer->Initialize();
		perlinBuffer->Bind();

		FBODescriptor perlinDesc;
		perlinDesc.dataType = GL_FLOAT;
		perlinDesc.format = gl_rg;
		perlinDesc.internalFormat = gl_rg16f;
		perlinDesc.internalFormat = gl_rg16f;
		perlinDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		perlinBuffer->AddAttachment(new frameBuffer::attachment_t("perlin", perlinDesc));

		frameBuffer::Unbind();

		heatHazeProgram = shaderPrograms[1]->handle;

		scene::InitializeUniforms();
		bubble.Initialize(2);
		perlin.Initialize(1);
	}

protected:

	void AddGUISpacer()
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::Checkbox("enable wireframe", &enableWireframe);
		
		ImGui::SliderFloat("Attenuation", &bubble.data.attenuation, 0.0f, 1.0f);
		ImGui::SliderFloat("grid dimensions", &bubble.data.gridDimensions, 0.0f, 1000.0f, "%.0f");
		ImGui::SliderFloat("offset", &bubble.data.offset, 0.0f, 1.0f);

		AddGUISpacer();

		ImGui::SliderFloat2("UV offset", &perlin.data.uvOffset[0], 0.0f, 10.0f);
		ImGui::SliderFloat2("UV scale", &perlin.data.uvScale[0], 100.0f, 1.0f);
		ImGui::SliderFloat("modifier value", &perlin.data.modValue, 0.0f, 1000.0f);
		ImGui::SliderFloat("permutation value", &perlin.data.permuteValue, 0.0f, 100.0f);
		ImGui::SliderFloat("taylor inverse", &perlin.data.taylorInverse, 0.0f, 10.0f);

		AddGUISpacer();

		ImGui::SliderFloat("fade value 1", &perlin.data.fadeValue1, 0.0f, 100.0f);
		ImGui::SliderFloat("fade value 2", &perlin.data.fadeValue2, 0.0f, 100.0f);
		ImGui::SliderFloat("fade value 3", &perlin.data.fadeValue3, 0.0f, 100.0f);

		AddGUISpacer();

		ImGui::SliderInt("num octaves", &perlin.data.numOctaves, 0, 100);

		AddGUISpacer();

		ImGui::SliderInt("color bias", &perlin.data.colorBias, 0, 100);

		AddGUISpacer();
		ImGui::SliderFloat("pattern value 1", &perlin.data.patternValue1, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 2", &perlin.data.patternValue2, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 3", &perlin.data.patternValue3, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 4", &perlin.data.patternValue4, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 5", &perlin.data.patternValue5, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 6", &perlin.data.patternValue6, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 7", &perlin.data.patternValue7, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 8", &perlin.data.patternValue8, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 9", &perlin.data.patternValue9, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 10", &perlin.data.patternValue10, 0.0f, 10.0f);
		ImGui::SliderFloat("pattern value 11", &perlin.data.patternValue11, 0.0f, 10.0f);

		ImGui::Begin("framebuffers");
		ImGui::Image((ImTextureID*)perlinBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();

	}

	frameBuffer* perlinBuffer;
	bufferHandler_t<perlinSettings_t>			perlin;
	unsigned int								heatHazeProgram;
	vertexBuffer_t*								perlinVBuffer;

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		bubble.Initialize(1);
		perlin.Initialize(2);
	}

	void SetupVertexBuffer() override
	{ 
		GLfloat cellWidth = defaultPayload.data.resolution.x / bubble.data.gridDimensions;
		GLfloat cellHeight = defaultPayload.data.resolution.y / bubble.data.gridDimensions;

		defaultVertexBuffer = new vertexBuffer_t(glm::vec2(cellWidth, cellHeight));

		perlinVBuffer = new vertexBuffer_t(defaultPayload.data.resolution);
	}

	virtual void Update() override
	{
		scene::Update();
		bubble.Update();
		perlin.Update();
	}

	void PerlinPass()
	{
		perlinBuffer->Bind();

		GLenum drawBuffers[1] =
		{
			gl_color_attachment0
		};

		glDrawBuffers(1, drawBuffers);

		glUseProgram(this->programGLID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		perlinBuffer->Unbind();
	}

	void HeatHazePass()
	{
		defaultTexture->GetUniformLocation(programGLID);
		defaultTexture->SetActive(0);
		perlinBuffer->attachments[0]->SetActive(1);
		glUseProgram(this->heatHazeProgram);
		if (enableWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)(bubble.data.gridDimensions * bubble.data.gridDimensions));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void Draw()	override
	{
		PerlinPass();
		HeatHazePass();

		texturedScene::DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void ClearBuffers()
	{
		//ok copy the current frame into the previous frame and clear the rest of the buffers	
		float clearColor1[4] = { 0.25f, 0.25f, 0.25f, 0.25f };

		perlinBuffer->Bind();
		perlinBuffer->ClearTexture(perlinBuffer->attachments[0], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		perlinBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::ivec2 resolution)
	{
		perlinBuffer->Resize(glm::ivec3(resolution, 1));
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		defaultPayload.data.resolution = glm::ivec2(dimensions.width, dimensions.height);
		ResizeBuffers(glm::ivec2(dimensions.x, dimensions.y));
		Resize(window, glm::ivec2(dimensions.x, dimensions.y));
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		defaultPayload.data.resolution = glm::ivec2(window->settings.resolution.width, window->settings.resolution.height);
		ResizeBuffers(defaultPayload.data.resolution);
		Resize(window, defaultPayload.data.resolution);
	}

	virtual void Resize(tWindow* window, glm::ivec2 dimensions = glm::ivec2(0)) override
	{
		scene::Resize(window, dimensions);

		perlinVBuffer->UpdateBuffer(dimensions);
		ResizeBuffers(dimensions);
	}
};

#endif
