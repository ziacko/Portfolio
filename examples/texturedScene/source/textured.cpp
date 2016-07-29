#include "TexturedScene.h"
//#include <stdio.h>
int main()
{
	texturedScene* exampleScene = new texturedScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}
