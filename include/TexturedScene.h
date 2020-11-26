#ifndef TEXTURED_SCENE_H
#define TEXTURED_SCENE_H
#include <Scene.h>
#include <Texture.h>

class texturedScene : public scene
{
public:

	texturedScene(texture* defaultTexture = new texture("../../resources/textures/earth_diffuse.tga"),
		const char* windowName = "Ziyad Barakat's Portfolio (textured scene)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Textured.txt") :
		scene(windowName, textureCamera, shaderConfigPath)
	{
		this->defaultTexture = defaultTexture;
		isGUIActive = false;
	}

	virtual void Initialize() override
	{
		scene::Initialize();
		defaultTexture->LoadTexture();
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene::BuildGUI(window, io);
	}

	void HandleFileDrop(tWindow* window, const std::vector<std::string>& files, const vec2_t<int>& windowMousePosition) override
	{
		//for each file that is dropped in
		//make sure its a texture 
		//and load up a new window for each one

		//first let's have it change the texture on display
		glFinish();
		defaultTexture->ReloadTexture(files[0].c_str());
	}

	virtual void Draw() override
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);
	
		defaultTexture->SetActive(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void SetupCallbacks() override
	{
		scene::SetupCallbacks();
		manager->fileDropEvent = std::bind(&texturedScene::HandleFileDrop, this, _1, _2, _3);
	}

protected:

	texture*							defaultTexture;
	std::vector<std::string>			textureDirs;
	int									currentTextureIndex;
	bool								isGUIActive;
};
#endif
