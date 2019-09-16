#ifndef MIPMAPPING_H
#define MIPMAPPING_H
#include "TextureSettings.h"

struct mipSettings_t
{
	int			mipLevel;
	GLuint		bufferHandle;
	GLuint		uniformHandle;

	mipSettings_t(GLuint mipLevel)
	{
		this->mipLevel = mipLevel;
		bufferHandle = 0;
		uniformHandle = 0;
	}

	~mipSettings_t() {};
};

class MipMappingScene : public textureSettings
{
public:
	//this was never finished so I'm going to leave it for last
	MipMappingScene(texture* defaultTexture = new texture("../../resources/textures/earth_diffuse.tga", "defaultTexture",
		texture::textureType_t::image, GL_RGBA, GL_TEXTURE_2D, 0, 10),
		const char* windowName = "Ziyad Barakat's Portfolio (mip mapping)",
		camera* edgeCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/MipMapping.txt")
		: textureSettings(defaultTexture, windowName, edgeCamera, shaderConfigPath)
	{
		mipSettingsBuffer = new mipSettings_t(0);
	}

	void Initialize() override
	{
		texturedScene::Initialize();
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::SliderInt("mip level", &mipSettingsBuffer->mipLevel, 0, 10);
		DrawTextureSettings();
	}

	void DrawTextureSettings()
	{
		//min
		if (ImGui::ListBox("min filter setting", &minFilterIndex, filterSettings.data(), filterSettings.size()))
		{
			glFinish();
			defaultTexture->SetMinFilter(minFilterIndex);
		}
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		SetupBuffer(mipSettingsBuffer, mipSettingsBuffer->bufferHandle, sizeof(*mipSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		scene::Update();
		SetupBuffer(mipSettingsBuffer, mipSettingsBuffer->bufferHandle, sizeof(*mipSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

protected:

	mipSettings_t*					mipSettingsBuffer;
};
#endif