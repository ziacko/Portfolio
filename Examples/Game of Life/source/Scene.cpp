#include "GameofLife.h"

int main()
{
	
	golScene* exampleScene = new golScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}