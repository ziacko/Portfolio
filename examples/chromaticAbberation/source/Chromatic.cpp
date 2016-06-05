#include "ChromaticAberration.h"

int main()
{
	//FreeImage_Initialise();
	chromaticScene* exampleScene = new chromaticScene();
	exampleScene->Initialize();
	exampleScene->Run();

	//FreeImage_DeInitialise();

	return 0;
}