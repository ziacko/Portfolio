#include "Parallax.h"

int main()
{
	parallaxScene* exampleScene = new parallaxScene(
		new parallaxSettings_t(),
		new texture("../../resources/textures/stones.bmp", "diffuseMap"), 
		new texture("../../resources/textures/stones_NM_height.tga", "heightMap"));

	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}