#ifndef GAUSSIAN_BLURRING_H
#define GAUSSIAN_BLURRING_H
#include <TexturedScene.h>
#include "FrameBuffer.h"

struct gaussianHorzSettings_t
{
	int			numOffsets;
	float		weight;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	gaussianHorzSettings_t(GLuint numOffsets = 5, GLfloat weight = 0.1f)
	{
		this->numOffsets = numOffsets;
		this->weight = weight;
	}

	~gaussianHorzSettings_t(){};
};

class gaussianMultiScene : public texturedScene
{
public:

	gaussianMultiScene(gaussianHorzSettings_t* gaussian = new gaussianHorzSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (gaussian blurring)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/GaussianMulti.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{
		this->gaussianSettingsBuffer = gaussian;
	}

	void Initialize() override
	{
		texturedScene::Initialize();
		
		verticalProgramHandle = shaderHandler->shaderPrograms[0]->handle;
		horizontalProgramHandle = shaderHandler->shaderPrograms[1]->handle;
		renderTextures = new frameBuffer();
		//add 2 render textures, one for the first pass and one for the second?
		//or just one and keep passing it through
		renderTextures->AddAttachment(new frameBuffer::attachment_t());
	}

	virtual void Draw() override
	{
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//ok first do the the vertical pass
		//then the horizontal pass
		//then draw the final image?
		renderTextures->Draw(0);
		renderTextures->Bind();
	
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(verticalProgramHandle);
		defaultTexture->GetUniformLocation(verticalProgramHandle);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		DrawGUI(windows[0]);
		
		//glClearBufferiv(GL_COLOR, 0, )

	}

	~gaussianMultiScene(void){};

protected:

	static	gaussianHorzSettings_t*		gaussianSettingsBuffer;
	frameBuffer*						renderTextures;

	unsigned int						verticalProgramHandle;
	unsigned int						horizontalProgramHandle;
	unsigned int						finalProgamHandle; //may not be necessary



	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::SliderFloat("weight", &gaussianSettingsBuffer->weight, 0.0f, 1.0f);
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		gaussianSettingsBuffer->
		SetupBuffer(gaussianSettingsBuffer, gaussianSettingsBuffer->bufferHandle, sizeof(*gaussianSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		UpdateBuffer(gaussianSettingsBuffer, gaussianSettingsBuffer->bufferHandle, sizeof(*gaussianSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);
	}
};

gaussianHorzSettings_t* gaussianMultiScene::gaussianSettingsBuffer = nullptr;

#endif