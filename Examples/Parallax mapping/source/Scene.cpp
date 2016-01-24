#include "Parallax.h"

int main()
{
	FreeImage_Initialise();
	parallaxScene* exampleScene = new parallaxScene();
	exampleScene->Initialize();
	exampleScene->Run();

	FreeImage_DeInitialise();


	return 0;
}