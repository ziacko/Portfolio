#include "DepthPrePass.h"

int main()
{
	DepthPrePassScene* exampleScene = new DepthPrePassScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}