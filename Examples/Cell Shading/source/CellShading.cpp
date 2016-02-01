#include "CellShadingScene.h"

int main()
{
	
	cellShadingScene* exampleScene = new cellShadingScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}