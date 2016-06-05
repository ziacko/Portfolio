#include "Erosion.h"

int main()
{
	
	erosionScene* exampleScene = new erosionScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}