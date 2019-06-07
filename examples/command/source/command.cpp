#include "command.h"

int main()
{
	commandScene* exampleScene = new commandScene();
	exampleScene->Initialize();
	exampleScene->Run();
	return 0;
}