#ifndef TEXTURE_SETTINGS_H
#define TEXTURE_SETTINGS_H
#include <TexturedScene.h>

typedef enum {LINEAR = 0, NEAREST, NEAREST_MIPMAP_NEAREST, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR} filterSettings_t;
typedef enum { CLAMP_TO_EDGE = 0, MIRROR_CLAMP_TO_EDGE, CLAMP_TO_BORDER, REPEAT, MIRRORED_REPEAT } wrapSettings_t;

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

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override //it's not virtual because i don't really want anything to inherit from it at this point. 
	{
		//texturedScene::BuildGUI(io);
		DrawTextureSettings();
	}

	virtual void DrawTextureSettings()
	{
		if (ImGui::ListBox("mag filter setting", &magFilterIndex, filterSettings.data(), filterSettings.size()))
		{
			defaultTexture->SetMagFilter(magFilterSetting);
		}

		//min
		if (ImGui::ListBox("min filter setting", &minFilterIndex, filterSettings.data(), filterSettings.size()))
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

	filterSettings_t				minFilterSetting = LINEAR;
	filterSettings_t				magFilterSetting = LINEAR;
	wrapSettings_t					wrapSSetting = CLAMP_TO_EDGE;
	wrapSettings_t					wrapTSetting = CLAMP_TO_EDGE;
	wrapSettings_t					wrapRSetting = CLAMP_TO_EDGE;

	//no need to edit these. but I can't make these const :(
	std::vector<const char*>		wrapSettings = { "clamp to edge", "mirror clamp to edge", "clamp to border", "repeat", "mirrored repeat" };
	std::vector<const char*>		filterSettings = { "linear", "nearest", "nearest mipmap nearest" , "nearest mipmap linear" , "linear mipmap nearest" , "linear mipmap linear" };
	//std::vector<const char*>		magFilterSettings = { "linear", "nearest" };


	//look into making these into std::pairs?
	int minFilterIndex = 0; //can't be a local variable
	int magFilterIndex = 0;

	int sWrapIndex = 0;
	int tWrapIndex = 0;
	int rWrapIndex = 0;
};

#endif
