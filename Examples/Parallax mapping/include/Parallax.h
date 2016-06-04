#ifndef PARALLAX_H
#define PARALLAX_H

#include <TexturedScene.h>

struct parallaxSettings_t
{
	float			scale;
	float			rayHeight;
	int				numSamples;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	parallaxSettings_t(GLfloat scale = 0.1f, GLfloat rayHeight = 0.25f, GLuint numSamples = 100)
	{
		this->scale = scale;
		this->rayHeight = rayHeight;
		this->numSamples = numSamples;
	}

	~parallaxSettings_t(){};
};

class parallaxScene : public texturedScene
{
public:

	parallaxScene(
		parallaxSettings_t* parallaxSettings = new parallaxSettings_t(),
		texture* defaultTexture = new texture(),
		texture* heightMap = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (parallax mapping)",
		camera* parallaxCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Parallax.txt") : 
		texturedScene(defaultTexture, windowName, parallaxCamera, shaderConfigPath)
	{
		this->parallaxSettingsBuffer = parallaxSettings;
		this->tweakBarName = windowName;
		this->heightMap = heightMap;
	}

	void Initialize() override
	{
		texturedScene::Initialize();
		heightMap->LoadTexture();
	}

	~parallaxScene(){};

protected:

	static parallaxSettings_t*		parallaxSettingsBuffer;
	texture*						heightMap;
	int								heightMapIndex = 0;

	void BuildGUI(ImGuiIO io) override
	{
		//this one's gonna be trickier
		scene::BuildGUI(io);

		std::vector<const char*> tempTextureDirs;
		for (unsigned int textureIndex = 0; textureIndex < textureDirs.size(); textureIndex++)
		{
			tempTextureDirs.push_back(textureDirs[textureIndex].c_str());
		}

		if (ImGui::ListBox("textures", &currentTextureIndex, tempTextureDirs.data(), tempTextureDirs.size()))
		{
			delete defaultTexture; //remove the old one from memory
			defaultTexture = new texture(tempTextureDirs[currentTextureIndex], "diffuseMap");
			defaultTexture->LoadTexture();
		}

		if (ImGui::ListBox("heightmap", &heightMapIndex, tempTextureDirs.data(), tempTextureDirs.size()))
		{
			delete heightMap; //remove the old one from memory
			heightMap = new texture(tempTextureDirs[heightMapIndex], "heightMap");
			heightMap->LoadTexture();
		}

		ImGui::SliderFloat("parallax scale", &parallaxSettingsBuffer->scale, 0.f, 10.0f);
		ImGui::SliderFloat("ray height", &parallaxSettingsBuffer->rayHeight, 0.0f, 10.0f);
		ImGui::SliderInt("num samples", &parallaxSettingsBuffer->numSamples, 0, 1000);

	}

	void InitializeBuffers() override
	{
		scene::InitializeBuffers();
		SetupBuffer(parallaxSettingsBuffer, parallaxSettingsBuffer->bufferHandle, sizeof(*parallaxSettingsBuffer), 1, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	void SetupParallaxUniforms()
	{
		parallaxSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "parallaxSettings");
		glUniformBlockBinding(this->programGLID, parallaxSettingsBuffer->uniformHandle, 1);
	}

	void bindTextures()
	{
		defaultTexture->GetUniformLocation(programGLID); //ok so heightmap is fine. just diffuse map is screwed
		heightMap->GetUniformLocation(programGLID);
	}

	void Draw() override
	{
		//UpdateBuffer<parallaxSettings_t>(parallaxSettingsBuffer, parallaxSettingsBuffer->bufferHandle);
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);
		defaultTexture->GetUniformLocation(this->programGLID);
		heightMap->GetUniformLocation(this->programGLID);
		glDrawArrays(GL_QUADS, 0, 4);
		DrawGUI(window->name);
		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

parallaxSettings_t* parallaxScene::parallaxSettingsBuffer = nullptr;




#endif