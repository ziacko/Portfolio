#include "Sharpen.h"

int main()
{
	
	sharpenScene* exampleScene = new sharpenScene(
		new sharpenSettings_t(), new texture("./textures/stalker.jpg"));
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}