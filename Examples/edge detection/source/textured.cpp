#include "EdgeDetection.h"

int main()
{
	
	edgeDetectionScene* exampleScene = new edgeDetectionScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}