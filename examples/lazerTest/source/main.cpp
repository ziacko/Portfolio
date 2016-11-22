#include "lazerTest.h"

int main()
{
	lazerScene* exampleScene = new lazerScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}