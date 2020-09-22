#ifndef MIPMAPPING_H
#define MIPMAPPING_H
#include "TextureSettings.h"

class MipMappingScene : public textureSettings
{
public:
	//this was never finished so I'm going to leave it for last
	MipMappingScene(texture* defaultTexture = new texture("../../resources/textures/earth_diffuse.tga",
		texture::textureType_t::image, "defaultTexture", textureDescriptor()),
		const char* windowName = "Ziyad Barakat's Portfolio (mip mapping)",
		camera* edgeCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/MipMapping.txt")
		: textureSettings(defaultTexture, windowName, edgeCamera, shaderConfigPath)
	{
		mip = bufferHandler_t<int>(0);
	}

	void Initialize() override
	{
		texturedScene::Initialize();
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::SliderInt("mip level", &mip.data, 0, 10);
		DrawTextureSettings();
	}

	void DrawTextureSettings() override
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
		mip.Initialize(1);
	}

	void Update() override
	{
		scene::Update();
		mip.Update();
	}

protected:

	bufferHandler_t<int>	mip;
};
#endif