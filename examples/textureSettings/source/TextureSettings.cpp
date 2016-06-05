#include "TextureSettings.h"

int main()
{
	textureSettings* exampleScene = new textureSettings();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();

	return 0;
}
