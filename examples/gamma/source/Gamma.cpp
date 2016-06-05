#include "Gamma.h"

int main()
{
	
	gammaScene* exampleScene = new gammaScene();
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}