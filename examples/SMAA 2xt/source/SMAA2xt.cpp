#include "SMAA2xt.h"

int main()
{
	SMAA2xt* exampleScene = new SMAA2xt();
	exampleScene->Initialize();
	exampleScene->Run();
	delete exampleScene;
	return 0;
}