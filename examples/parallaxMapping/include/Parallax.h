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

	parallaxSettings_t*		parallaxSettingsBuffer = nullptr;
	texture*						heightMap;
	int								heightMapIndex = 0;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		//this one's gonna be trickier
		scene::BuildGUI(window, io);

		std::vector<const char*> tempTextureDirs;
		for (auto & textureDir : textureDirs)
		{
			tempTextureDirs.push_back(textureDir.c_str());
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
		scene::InitializeUniforms();
		SetupBuffer(parallaxSettingsBuffer, parallaxSettingsBuffer->bufferHandle, sizeof(*parallaxSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
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

	void Update() override
	{
		scene::Update();
		//UpdateBuffer()
		//UpdateBuffer(pixelizeSettings, pixelizeSettings->bufferHandle, sizeof(*pixelizeSettings), gl_uniform_buffer, gl_dynamic_draw);
		UpdateBuffer(parallaxSettingsBuffer, parallaxSettingsBuffer->bufferHandle, sizeof(*parallaxSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);
	}

	void Draw() override
	{
		
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);
		defaultTexture->GetUniformLocation(this->programGLID);
		heightMap->GetUniformLocation(this->programGLID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
#endif