#include "bubble.h"

int main()
{
	bubbleScene* bubble = new bubbleScene();
	bubble->Initialize();
	bubble->Run();

	bubble->ShutDown();


	return 0;
}
