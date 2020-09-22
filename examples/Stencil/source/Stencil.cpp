#include "Stencil.h"

int main()
{
	stencil* exampleScene = new stencil();
	exampleScene->Initialize();
	exampleScene->Run();
	return 0;
}