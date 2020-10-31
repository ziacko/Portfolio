#include "Profiler.h"

//MICROPROFILE_DEFINE(MAIN, "MAIN", "Main", 0xff0000);

int main()
{
	MicroProfileOnThreadCreate("Main");
	profiler* exampleScene = new profiler();
	exampleScene->Initialize();
	exampleScene->Run();
	MicroProfileContextSwitchTraceStop();
	MicroProfileShutdown();
	return 0;
}