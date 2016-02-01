#ifndef TEXTURED_SCENE_H
#define TEXTURED_SCENE_H
#include <Scene.h>
#include <Texture.h>

class texturedScene : public scene
{
public:

	texturedScene(texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (textured scene)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "./shaders/Textured.txt") :
		scene(windowName, textureCamera, shaderConfigPath)
	{
		this->defaultTexture = defaultTexture;
	}

	virtual void Initialize() override
	{
		scene::Initialize();
		defaultTexture->LoadTexture();
	}

	virtual void Draw() override
	{
		glBindVertexArray(this->vertexArrayObject);
		glUseProgram(this->programGLID);
		defaultTexture->GetUniformLocation(this->programGLID);

		glDrawArrays(GL_QUADS, 0, 4);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

protected:

	static texture*		defaultTexture;

};

texture* texturedScene::defaultTexture = nullptr;


#endif