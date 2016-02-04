#include "Sepia.h"

int main()
{
	
	sepiaScene* exampleScene = new sepiaScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}