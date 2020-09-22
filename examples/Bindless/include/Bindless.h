#ifndef BINDLESS_H
#define BINDLESS_H
#include <TexturedScene.h>

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
		bindless = bufferHandler_t<GLuint>(0);
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

	bufferHandler_t<GLuint>		bindless;

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		bindless.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		bindless.data = defaultTexture->GetResidentHandle();
		bindless.Update(gl_uniform_buffer, gl_dynamic_draw);
	}
};

#endif