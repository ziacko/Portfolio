#include "PerlinNoise3D.h"

int main()
{
	perlinScene3D* perlin = new perlinScene3D();
	perlin->Initialize();
	perlin->Run();


	return 0;
}