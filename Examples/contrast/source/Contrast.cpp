#include "Contrast.h"

int main()
{
	
	contrastScene* exampleScene = new contrastScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}