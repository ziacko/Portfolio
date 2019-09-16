#ifndef BINDLESS_H
#define BINDLESS_H
#include <TexturedScene.h>

//need to update Tinyextender to make this work properly
struct bindlessTextures_t
{
	GLuint		defaultTex;

	GLuint		bufferHandle;
	GLuint		uniformHandle;

	bindlessTextures_t(GLuint defaultTex)
	{
		this->defaultTex = defaultTex;
		bufferHandle = 0;
		uniformHandle = 0;
	}

	~bindlessTextures_t() {};
};

class Bindless : public texturedScene
{
public:

	Bindless(
		const char* windowName = "Ziyad Barakat's portfolio (Bindless texture)",
		texture* defaultTexture = new texture(),
		camera* cam = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Bindless.txt")
		: texturedScene(defaultTexture, windowName, cam, shaderConfigPath)
	{
		bindlessTex = new bindlessTextures_t(0);
	}

	~Bindless(){};

	virtual void Initialize() override
	{
		texturedScene::Initialize();		
		defaultTexture->ToggleResident();
	}

	virtual void Draw() override
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

protected:

	bindlessTextures_t*			bindlessTex;

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();		
		SetupBuffer(bindlessTex, bindlessTex->bufferHandle, sizeof(*bindlessTex), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		bindlessTex->defaultTex = defaultTexture->GetResidentHandle();
		UpdateBuffer(bindlessTex, bindlessTex->bufferHandle, sizeof(*bindlessTex), gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif