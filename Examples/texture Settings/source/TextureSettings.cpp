#include "TextureSettings.h"

int main()
{
	
	textureSettings* exampleScene = new textureSettings(new texture("./textures/crate_sideup.png"));
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();

	return 0;
}