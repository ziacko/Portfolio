#include "framebufferScene.h"

int main()
{
	framebufferScene* exampleScene = new framebufferScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}