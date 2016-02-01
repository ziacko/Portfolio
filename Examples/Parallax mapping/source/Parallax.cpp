#include "Parallax.h"

int main()
{
	parallaxScene* exampleScene = new parallaxScene(
		new parallaxSettings_t(),
		new texture("./textures/rocks.jpg", "diffuseMap"), 
		new texture("./textures/rocks_NM_height.tga", "heightMap"));

	exampleScene->Initialize();
	exampleScene->Run();
	exampleScene->ShutDown();

	return 0;
}