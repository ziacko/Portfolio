#include <radialBlur.h>

int main()
{
	radialScene* radial = new radialScene();
	radial->Initialize();
	radial->Run();

	radial->ShutDown();

	return 0;
}
