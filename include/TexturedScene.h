#ifndef TEXTURED_SCENE_H
#define TEXTURED_SCENE_H
#include <Scene.h>
#include <Texture.h>

class texturedScene : public scene
{
public:

	texturedScene(texture* defaultTexture = new texture("../../resources/textures/crate_sideup.png", "defaultTexture", GL_RGBA, GL_RGBA),
		const char* windowName = "Ziyad Barakat's Portfolio (textured scene)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Textured.txt") :
		scene(windowName, textureCamera, shaderConfigPath)
	{
		this->defaultTexture = defaultTexture;
		//FreeImage_Initialise();

		//find all textures in dir
		//GetFilesInDirectory(textureDirs, "../../resources/textures");
	}

	virtual void Initialize() override
	{
		scene::Initialize();
		defaultTexture->LoadTexture();
	}

	virtual void BuildGUI(ImGuiIO io) override
	{
		scene::BuildGUI(io);

		/*std::vector<const char*> tempTextureDirs;
		for (unsigned int textureIndex = 0; textureIndex < textureDirs.size(); textureIndex++)
		{
			tempTextureDirs.push_back(textureDirs[textureIndex].c_str());
		}
		
		if (ImGui::ListBox("textures", &currentTextureIndex, tempTextureDirs.data(), tempTextureDirs.size()))
		{
			delete defaultTexture; //remove the old one from memory
			defaultTexture = new texture(tempTextureDirs[currentTextureIndex]);
			defaultTexture->LoadTexture();
		}*/
	}

	virtual void Draw() override
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);
		defaultTexture->GetUniformLocation(this->programGLID);
		glDrawArrays(GL_QUADS, 0, 4);

		DrawGUI(window->name);

		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

protected:

	texture*							defaultTexture;
	std::vector<std::string>			textureDirs;
	int									currentTextureIndex;
};
#endif
