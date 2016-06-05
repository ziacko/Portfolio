#include "Sharpen.h"

int main()
{
	
	sharpenScene* exampleScene = new sharpenScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}