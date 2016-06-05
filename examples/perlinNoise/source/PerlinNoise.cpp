#include "PerlinNoise.h"

int main()
{
	perlinScene* perlin = new perlinScene();
	perlin->Initialize();
	perlin->Run();


	return 0;
}