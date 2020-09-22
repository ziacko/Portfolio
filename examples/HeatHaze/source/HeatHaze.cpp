#include "HeatHaze.h"

int main()
{
	heatHazeScene* heat = new heatHazeScene();
	heat->Initialize();
	heat->Run();

	return 0;
}
