#include "KHR_Viewer.h"
//#include <stdio.h>
int main()
{
	KHRScene* exampleScene = new KHRScene();
	exampleScene->Initialize();
	exampleScene->Run();

	return 0;
}
