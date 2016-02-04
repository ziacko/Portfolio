#include "Gaussian.h"

int main()
{
	
	gaussianScene* exampleScene = new gaussianScene(new gaussianSettings_t(), 
		new texture("./textures/crate_sideup.png"));
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}