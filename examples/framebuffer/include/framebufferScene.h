#ifndef EROSION_H
#define EROSION_H
#include <TexturedScene.h>
#include "FrameBuffer.h"

struct erosionSettings_t
{
	float			strengthX;
	float			strengthY;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	erosionSettings_t(
		GLfloat strengthX = 1.0f, GLfloat strengthY= 1.0f)
	{
		this->strengthX = strengthX;
		this->strengthY = strengthY;
	}

	~erosionSettings_t(){};
};

class framebufferScene : public texturedScene
{
public:

	framebufferScene(
		erosionSettings_t* erosionSettings = new erosionSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (framebuffer)",
		camera* erosionCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/FrameBuffer.txt")
		: texturedScene(defaultTexture, windowName, erosionCamera, shaderConfigPath)
	{
		this->erosionSettings = erosionSettings;
		sceneBuffer = new frameBuffer();
	}

	~framebufferScene() {};

	virtual void Initialize() override
	{
		texturedScene::Initialize();
		defaultTexture->LoadTexture();
		sceneBuffer->Initialize();
		sceneBuffer->AddAttachment("test", 
			new frameBuffer::attachment_t(frameBuffer::attachment_t::attachmentType_t::color,
				"defaultTexture", glm::vec2(windows[0]->resolution.width, windows[0]->resolution.height)));
		sceneBuffer->Unbind();
		bufferProgram = shaderPrograms[1]->handle;
	}

protected:

	erosionSettings_t*		erosionSettings;
	frameBuffer* sceneBuffer;
	unsigned int bufferProgram = 0;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(windows[0], io);
		ImGui::SliderFloat("erosion strength X", &erosionSettings->strengthX, 0.0f, 10.0f);
		ImGui::SliderFloat("erosion strength Y", &erosionSettings->strengthY, 0.0f, 10.0f);
		ImGui::Image((ImTextureID*)sceneBuffer->attachments["test"]->GetHandle(), ImVec2(160, 120),
			ImVec2(0, 1), ImVec2(1, 0));
	}

	void InitializeBuffers() override
	{
		scene::InitializeUniforms();
		SetupBuffer(erosionSettings, erosionSettings->bufferHandle, sizeof(*erosionSettings), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(erosionSettings, erosionSettings->bufferHandle, sizeof(*erosionSettings), gl_uniform_buffer, gl_dynamic_draw);
	}

	void Draw() override
	{
		sceneBuffer->Bind();

		sceneBuffer->Draw("test");
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);

		defaultTexture->SetActive();
		glUseProgram(this->programGLID);
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		defaultTexture->UnbindTexture();
		sceneBuffer->Unbind();

		//draw to backbuffer
		sceneBuffer->attachments["test"]->OverloadTextureUnit(0);
		glUseProgram(bufferProgram);
		glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		sceneBuffer->attachments["test"]->UnbindTexture();

		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		sceneBuffer->ClearTexture(sceneBuffer->attachments["test"]);
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions) override
	{
		glViewport(0, 0, dimensions.width, dimensions.height);
		defaultUniform->resolution = glm::vec2(dimensions.width, dimensions.height);
		defaultUniform->projection = glm::ortho(0.0f, (GLfloat)dimensions.width, (GLfloat)dimensions.height, 0.0f, 0.01f, 10.0f);

		sceneBuffer->Bind();
		sceneBuffer->attachments["test"]->Resize(glm::vec2(dimensions.x, dimensions.y));
		sceneBuffer->Unbind();

		UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
		defaultVertexBuffer->UpdateBuffer(defaultUniform->resolution);
	}

	virtual void HandleMaximize(tWindow* window) override
	{
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		defaultUniform->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		defaultUniform->projection = glm::ortho(0.0f, (GLfloat)window->resolution.width, (GLfloat)window->resolution.height, 0.0f, 0.01f, 10.0f);

		sceneBuffer->Bind();
		sceneBuffer->attachments["test"]->Resize(glm::vec2(window->resolution.width, window->resolution.height));
		sceneBuffer->Unbind();

		//bind the uniform buffer and refill it
		glBindBuffer(gl_uniform_buffer, defaultUniform->bufferHandle);
		glBufferData(gl_uniform_buffer, sizeof(*defaultUniform), defaultUniform, gl_dynamic_draw);

		defaultVertexBuffer->UpdateBuffer(defaultUniform->resolution);
	}
};

#endif