#include "scene3D.h"

int main()
{
	scene3D* exampleScene = new scene3D();
	exampleScene->Initialize();
	exampleScene->Run();
	return 0;
}