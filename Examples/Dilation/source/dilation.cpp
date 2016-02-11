#include "Dilation.h"

int main()
{
	
	dilationScene* exampleScene = new dilationScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}