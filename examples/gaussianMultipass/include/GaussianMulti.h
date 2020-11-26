#ifndef GAUSSIAN_BLURRING_H
#define GAUSSIAN_BLURRING_H
#include <TexturedScene.h>
#include "FrameBuffer.h"

struct gaussianSettings_t
{
	int			offsets[5];
	GLfloat		weights[5];

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	gaussianSettings_t()
	{
		for(size_t iter = 0; iter < 5; iter++)
		{
			offsets[iter] = iter;
		}

		weights[0] = 0.2270270270;
		weights[1] = 0.1945945946;
		weights[2] = 0.1216216216;
		weights[3] = 0.0540540541;
		weights[4] = 0.0162162162;
	};

	~gaussianSettings_t(){};
};

class gaussianMultiScene : public texturedScene
{
public:

	gaussianMultiScene(texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (gaussian blurring)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/GaussianMulti.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		gaussianHorz.data = gaussianSettings_t();
		gaussianVert.data = gaussianSettings_t();
		gaussBuffer = new frameBuffer();
		compareBuffer = new frameBuffer();
	}

	void Initialize() override
	{
		texturedScene::Initialize();
		
		verticalProgram = shaderHandler->shaderPrograms[0]->handle;
		horizontalProgram = shaderHandler->shaderPrograms[1]->handle;
		finalProgam = shaderHandler->shaderPrograms[2]->handle;
		compareProgram = shaderHandler->shaderPrograms[3]->handle;
		
		gaussBuffer->Initialize();
		gaussBuffer->Bind();

		FBODescriptor gaussDesc;
		gaussDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		//add 2 render textures, one for the first pass and one for the second?
		//or just one and keep passing it through
		gaussBuffer->AddAttachment(new frameBuffer::attachment_t("vertical", gaussDesc));
		gaussBuffer->AddAttachment(new frameBuffer::attachment_t("horizontal", gaussDesc));

		compareBuffer->Initialize();
		compareBuffer->Bind();

		compareBuffer->AddAttachment(new frameBuffer::attachment_t("compare", gaussDesc));

		frameBuffer::Unbind();
	}

	void VerticalPass()
	{
		gaussBuffer->Bind();
		gaussBuffer->attachments[0]->Draw();

		defaultTexture->SetActive(0);

		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(verticalProgram);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void HorizontalPass()
	{
		gaussBuffer->Bind();
		gaussBuffer->attachments[1]->Draw();

		defaultTexture->SetActive(0);

		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(horizontalProgram);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void BlendPass()
	{
		//draw to backbuffer
		compareBuffer->Bind();
		compareBuffer->attachments[0]->Draw();

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		gaussBuffer->attachments[0]->SetActive(0);
		gaussBuffer->attachments[1]->SetActive(1);

		glUseProgram(finalProgam);

		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	virtual void FinalPass(texture* tex1, frameBuffer::attachment_t* tex2)
	{
		//draw directly to backbuffer
		frameBuffer::Unbind();
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		tex1->SetActive(0);
		tex2->SetActive(1);

		glUseProgram(compareProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	virtual void Draw() override
	{
		//ok first do the the vertical pass
		//then the horizontal pass
		//then draw the final image?
		VerticalPass();

		HorizontalPass();

		BlendPass();

		FinalPass(defaultTexture, compareBuffer->attachments[0]);

		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	}

	~gaussianMultiScene(void){};

protected:

	bufferHandler_t<gaussianSettings_t>		gaussianHorz;
	bufferHandler_t<gaussianSettings_t>		gaussianVert;
	
	frameBuffer* gaussBuffer;
	frameBuffer* compareBuffer;

	unsigned int						verticalProgram;
	unsigned int						horizontalProgram;
	unsigned int						finalProgam;
	unsigned int						compareProgram;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::Begin("horizontal");
		for(size_t iter = 0; iter < 5; iter++)
		{
			std::string num = std::to_string(iter);
			ImGui::DragInt(std::string("offset# " + num).c_str(),
				&gaussianHorz.data.offsets[iter], 1.0f, 0.0f, 10.0f);
		}
		ImGui::Separator();
		for (size_t iter = 0; iter < 5; iter++)
		{
			std::string num = std::to_string(iter);
			ImGui::DragFloat(std::string("weight# " + num).c_str(),
				&gaussianHorz.data.weights[iter], 0.001f, 0.0f, 1.0f);
		}
		ImGui::End();

		ImGui::Begin("vertical");
		for (size_t iter = 0; iter < 5; iter++)
		{
			std::string num = std::to_string(iter);
			ImGui::DragInt(std::string("offset# " + num).c_str(),
				&gaussianVert.data.offsets[iter], 1.0f, 0.0f, 10.0f);
		}
		ImGui::Separator();
		for (size_t iter = 0; iter < 5; iter++)
		{
			std::string num = std::to_string(iter);
			ImGui::DragFloat(std::string("weight# " + num).c_str(),
				&gaussianVert.data.weights[iter], 0.001f, 0.0f, 1.0f);
		}
		ImGui::End();
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		gaussianHorz.Initialize(1);
		gaussianVert.Initialize(2);
		//SetupBuffer(gaussian, gaussian->bufferHandle, sizeof(*gaussian), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		gaussianHorz.Update();
		gaussianVert.Update();
		//UpdateBuffer(gaussian, gaussian->bufferHandle, sizeof(*gaussian), gl_uniform_buffer, gl_dynamic_draw);
	}

	void ClearBuffers()
	{
		float clearColor1[4] = { 0.25f, 0.25f, 0.25f, 0.25f };

		gaussBuffer->Bind();
		gaussBuffer->ClearTexture(gaussBuffer->attachments[0], clearColor1);
		//geometryBuffer->ClearTexture(geometryBuffer->attachments[1], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		frameBuffer::Unbind();
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

	virtual void ResizeBuffers(glm::ivec2 resolution)
	{
		gaussBuffer->Resize(glm::ivec3(resolution, 1));
		compareBuffer->Resize(glm::ivec3(resolution, 1));
	}

	virtual void Resize(tWindow* window, glm::ivec2 dimensions  = glm::ivec2(0))
	{
		scene::Resize(window, dimensions);
		ResizeBuffers(dimensions);

	}
};

#endif