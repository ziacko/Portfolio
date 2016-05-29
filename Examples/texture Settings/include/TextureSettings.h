#ifndef TEXTURE_SETTINGS_H
#define TEXTURE_SETTINGS_H
#include <TexturedScene.h>
//can't use SOIL with this. I need to get FreeImage working again
typedef enum { LINEAR = 0, NEAREST } magFilterSettings_t;
typedef enum { NEAREST_MIPMAP_NEAREST = 2, NEAREST_MIPMAP_LINEAR, LINEAR_MIPMAP_NEAREST, LINEAR_MIPMAP_LINEAR } minFilterSettings_t;
typedef enum { CLAMP_TO_EDGE = 0, MIRROR_CLAMP_TO_EDGE, CLAMP_TO_BORDER, REPEAT, MIRRORED_REPEAT } wrapSettings_t;

class textureSettings : public texturedScene
{
public:
	textureSettings(texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (texture settings)",
		camera* textureCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/textureSettings.txt") :
		texturedScene(defaultTexture, windowName, textureCamera, shaderConfigPath)
	{

	}

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwEnumVal magFilterEV[] = { { LINEAR, "linear" }, { NEAREST, "Nearest" } };
		TwType magFilterType = TwDefineEnum("filterSetting", magFilterEV, 2);

		TwEnumVal wrapSettingEV[] = { { CLAMP_TO_EDGE, "clamp to edge" }, { MIRROR_CLAMP_TO_EDGE, "mirrored clamp to edge" },
		{ CLAMP_TO_BORDER, "clamp to border" }, { REPEAT, "repeat" }, { MIRRORED_REPEAT, "mirrored repeat" } };
		TwType wrapSettingType = TwDefineEnum("wrap setting", wrapSettingEV, 5);

		/ *TwEnumVal minfilterEV[] = { { LINEAR, "linear" }, { NEAREST, "Nearest" },
		{ NEAREST_MIPMAP_NEAREST, "nearest mipmap nearest" }, { NEAREST_MIPMAP_LINEAR, "nearest mipmap linear" },
		{ LINEAR_MIPMAP_NEAREST, "linear mipmap nearest" }, { LINEAR_MIPMAP_LINEAR, "linear mipmap linear" } };* /

		TwAddVarCB(tweakBar, "min filter settings", magFilterType, SetMinFilter, GetMinFilter, &minFilterSetting, NULL);
		TwAddVarCB(tweakBar, "mag filter settings", magFilterType, SetMagFilter, GetMagFilter, &magFilterSetting, NULL);

		TwAddVarCB(tweakBar, "wrap S setting", wrapSettingType, SetWrapSSettings, GetWrapSSetting, &wrapSSetting, NULL);
		TwAddVarCB(tweakBar, "wrap T setting", wrapSettingType, SetWrapTSettings, GetWrapTSetting, &wrapTSetting, NULL);
		TwAddVarCB(tweakBar, "wrap R setting", wrapSettingType, SetWrapRSettings, GetWrapRSetting, &wrapRSetting, NULL);
	}

	static void TW_API SetMagFilter(const void* value, void* clientData)
	{
		magFilterSetting = *(const magFilterSettings_t *)(value);
		defaultTexture->SetMagFilter(magFilterSetting);
	}

	static void TW_API GetMagFilter(void* value, void* clientData)
	{
		*(magFilterSettings_t *)value = magFilterSetting;
	}

	static void TW_API SetMinFilter(const void* value, void* clientData)
	{
		minFilterSetting = *(const magFilterSettings_t *)(value);
		defaultTexture->SetMinFilter(minFilterSetting);
	}

	static void TW_API GetMinFilter(void* value, void* clientData)
	{
		*(magFilterSettings_t *)value = minFilterSetting;
	}

	static void TW_API SetWrapSSettings(const void* value, void* clientData)
	{
		wrapSSetting = *(const wrapSettings_t*)(value);
		defaultTexture->SetWrapS(wrapSSetting);
	}

	static void TW_API GetWrapSSetting(void* value, void* clientData)
	{
		*(wrapSettings_t *)value = wrapSSetting;
	}

	static void TW_API SetWrapTSettings(const void* value, void* clientData)
	{
		wrapTSetting = *(const wrapSettings_t*)(value);
		defaultTexture->SetWrapT(wrapTSetting);
	}

	static void TW_API GetWrapTSetting(void* value, void* clientData)
	{
		*(wrapSettings_t *)value = wrapTSetting;
	}

	static void TW_API SetWrapRSettings(const void* value, void* clientData)
	{
		wrapRSetting = *(const wrapSettings_t*)(value);
		defaultTexture->SetWrapR(wrapRSetting);
	}

	static void TW_API GetWrapRSetting(void* value, void* clientData)
	{
		*(wrapSettings_t *)value = wrapRSetting;
	}*/

protected:

	static magFilterSettings_t			minFilterSetting;
	static magFilterSettings_t			magFilterSetting;
	static wrapSettings_t				wrapSSetting;
	static wrapSettings_t				wrapTSetting;
	static wrapSettings_t				wrapRSetting;
};

magFilterSettings_t textureSettings::minFilterSetting = NEAREST;
magFilterSettings_t textureSettings::magFilterSetting = NEAREST;

wrapSettings_t textureSettings::wrapSSetting = REPEAT;
wrapSettings_t textureSettings::wrapTSetting = CLAMP_TO_EDGE;
wrapSettings_t textureSettings::wrapRSetting = CLAMP_TO_EDGE;

#endif