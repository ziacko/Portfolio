#include "SMAA1xt.h"

int main()
{
	SMAA1xt* exampleScene = new SMAA1xt();
	exampleScene->Initialize();
	exampleScene->Run();
	delete exampleScene;
	return 0;
}