#include "SMAA.h"

int main()
{
	SMAA* exampleScene = new SMAA();
	exampleScene->Initialize();
	exampleScene->Run();
	delete exampleScene;
	return 0;
}