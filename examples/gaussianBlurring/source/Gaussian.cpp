#include "Gaussian.h"

int main()
{
	
	gaussianScene* exampleScene = new gaussianScene(new gaussianSettings_t(), 
		new texture("../../resources/textures/crate_sideup.png"));
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}