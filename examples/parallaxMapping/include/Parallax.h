#ifndef PARALLAX_H
#define PARALLAX_H

#include <TexturedScene.h>

struct parallax_t
{
	float			scale;
	float			rayHeight;
	int				numSamples;

	parallax_t(GLfloat scale = 0.1f, GLfloat rayHeight = 0.25f, GLuint numSamples = 100)
	{
		this->scale = scale;
		this->rayHeight = rayHeight;
		this->numSamples = numSamples;
	}

	~parallax_t(){};
};

class parallaxScene : public texturedScene
{
public:

	parallaxScene(
		bufferHandler_t<parallax_t> parallaxSettings = bufferHandler_t<parallax_t>(),
		texture* defaultTexture = new texture(),
		texture* heightMap = new texture(),
		const char* windowName = "Ziyad Barakat's portfolio (parallax mapping)",
		camera* parallaxCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Parallax.txt") : 
		texturedScene(defaultTexture, windowName, parallaxCamera, shaderConfigPath)
	{
		this->parallax = parallaxSettings;
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

	bufferHandler_t<parallax_t>		parallax;
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
			defaultTexture = new texture(tempTextureDirs[currentTextureIndex], texture::textureType_t::diffuse, "diffuseMap", textureDescriptor());
			defaultTexture->LoadTexture();
		}

		if (ImGui::ListBox("heightmap", &heightMapIndex, tempTextureDirs.data(), tempTextureDirs.size()))
		{
			delete heightMap; //remove the old one from memory
			heightMap = new texture(tempTextureDirs[heightMapIndex], texture::textureType_t::diffuse, "heightMap", textureDescriptor());
			heightMap->LoadTexture();
		}

		ImGui::SliderFloat("parallax scale", &parallax.data.scale, 0.f, 10.0f);
		ImGui::SliderFloat("ray height", &parallax.data.rayHeight, 0.0f, 10.0f);
		ImGui::SliderInt("num samples", &parallax.data.numSamples, 0, 1000);

	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		parallax.Initialize(1);
	}

	void SetupParallaxUniforms()
	{
		parallax.Initialize(1);
		glUniformBlockBinding(this->programGLID, parallax.uniformHandle, 1);
	}

	void bindTextures()
	{
		defaultTexture->GetUniformLocation(programGLID); //ok so heightmap is fine. just diffuse map is screwed
		heightMap->GetUniformLocation(programGLID);
	}

	void Update() override
	{
		scene::Update();
		parallax.Update();
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