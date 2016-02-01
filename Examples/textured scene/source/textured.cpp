#include "TexturedScene.h"

int main()
{
	
	texturedScene* exampleScene = new texturedScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}