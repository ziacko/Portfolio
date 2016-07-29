#include "pixelize.h"

int main()
{
	pixelizeScene* exampleScene = new pixelizeScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}