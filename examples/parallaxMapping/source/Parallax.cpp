#include "Parallax.h"

int main()
{
	parallaxScene* exampleScene = new parallaxScene(
		bufferHandler_t<parallax_t>(),
		new texture("../../resources/textures/stones.bmp", texture::textureType_t::diffuse, "diffuseMap"), 
		new texture("../../resources/textures/stones_NM_height.tga", texture::textureType_t::diffuse, "heightMap"));

	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}