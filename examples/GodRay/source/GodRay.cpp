#include "GodRay.h"

int main()
{
	GodRayScene* exampleScene = new GodRayScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}