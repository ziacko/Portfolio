#include "GaussianMulti.h"

int main()
{
	
	gaussianMultiScene* exampleScene = new gaussianMultiScene(new texture("../../resources/textures/crate_sideup.png"));
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}