#include "PhongLighting.h"

int main()
{
	PhongLighting* exampleScene = new PhongLighting();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}