#include "MipMapping.h"

int main()
{
	textureDescriptor texDesc = textureDescriptor();
	texDesc.target = GL_TEXTURE_2D;
	texDesc.mipmapLevels = 10;

	MipMappingScene* exampleScene = new MipMappingScene(new texture("../../resources/textures/earth_diffuse.tga",
		texture::textureType_t::image, "defaultTexture", texDesc));
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}