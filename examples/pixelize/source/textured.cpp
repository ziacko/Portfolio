#include "pixelize.h"

int main()
{
	
	pixelizeScene* exampleScene = new pixelizeScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}