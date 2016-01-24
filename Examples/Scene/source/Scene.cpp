#include <Scene.h>

int main()
{
	
	scene* exampleScene = new scene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}