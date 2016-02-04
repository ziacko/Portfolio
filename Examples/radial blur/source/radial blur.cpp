#include "radialBlur.h"

int main()
{
	
	radialScene* exampleScene = new radialScene(new radialBlur_t());
	exampleScene->Initialize();
	exampleScene->Run();

	exampleScene->ShutDown();


	return 0;
}