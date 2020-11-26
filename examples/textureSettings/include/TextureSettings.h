#ifndef TEXTURE_SETTINGS_H
#define TEXTURE_SETTINGS_H
#include <TexturedScene.h>

using magFilterSettings_t = enum { LINEAR = 0, NEAREST };
using minFilterSettings_t = enum { NEAREST_MIPMAP_NEAREST = 2, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR };
using wrapSettings_t = enum { CLAMP_TO_EDGE = 0, MIRROR_CLAMP_TO_EDGE, CLAMP_TO_BORDER, REPEAT, MIRRORED_REPEAT };

class textureSettings : public texturedScene
{
public:
	textureSettings(texture* defaultTexture = new texture("../../resources/textures/crate_sideup.png"),
		const char* windowName = "Ziyad Barakat's Portfolio (texture settings)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/TextureSettings.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{

	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		//texturedScene::BuildGUI(io);
		DrawTextureSettings();

	}

	void DrawTextureSettings()
	{
		if (ImGui::ListBox("mag filter setting", &magFilterIndex, magFilterSettings.data(), magFilterSettings.size()))
		{
			defaultTexture->SetMagFilter(magFilterSetting);
		}

		//min
		if (ImGui::ListBox("min filter setting", &minFilterIndex, minFilterSettings.data(), minFilterSettings.size()))
		{
			defaultTexture->SetMinFilter(minFilterIndex);
		}

		//S wrap setting
		if (ImGui::ListBox("S wrap texture setting", &sWrapIndex, wrapSettings.data(), wrapSettings.size()))
		{
			defaultTexture->SetWrapS(sWrapIndex);
		}
		//T wrap setting
		if (ImGui::ListBox("T wrap texture setting", &tWrapIndex, wrapSettings.data(), wrapSettings.size()))
		{
			defaultTexture->SetWrapT(tWrapIndex);
		}
		//R wrap setting (3D textures) //not doing 3D right now so I'll leave it out
		/*if (ImGui::ListBox("R wrap texture setting", &rWrapIndex, wrapSettings.data(), wrapSettings.size()))
		{
			defaultTexture->SetWrapR(rWrapIndex);
		}*/
	}

protected:

	magFilterSettings_t				minFilterSetting = LINEAR;
	magFilterSettings_t				magFilterSetting = LINEAR;
	wrapSettings_t					wrapSSetting = CLAMP_TO_EDGE;
	wrapSettings_t					wrapTSetting = CLAMP_TO_EDGE;
	wrapSettings_t					wrapRSetting = CLAMP_TO_EDGE;

	//no need to edit these. but I can't make these const :(
	std::vector<const char*>		wrapSettings = { "clamp to edge", "mirror clamp to edge", "clamp to border", "repeat", "mirrored repeat" };
	std::vector<const char*>		magFilterSettings = { "linear", "nearest" };
	std::vector<const char*>		minFilterSettings = { "linear", "nearest", "nearest mipmap nearest" , "nearest mipmap linear" , "linear mipmap nearest" , "linear mipmap linear" };

	//look into making these into std::pairs?
	int minFilterIndex = 0; //can't be a local variable
	int magFilterIndex = 0;

	int sWrapIndex = 0;
	int tWrapIndex = 0;
	int rWrapIndex = 0;
};

#endif
