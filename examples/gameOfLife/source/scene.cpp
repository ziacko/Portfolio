#include "gameOfLife.h"

int main()
{
	golScene* exampleScene = new golScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}
