#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H
#include <TexturedScene.h>

typedef enum {SOBEL = 0, LAPLACIAN, PREWITT, FREI_CHEN} edgeFilters_t; //ooh right this was never finished. crap!

struct sobelSettings_t
{
	GLuint				bufferHandle;
	GLuint				uniformHandle;
	edgeFilters_t		edgeFilter;

	sobelSettings_t()
	{
		edgeFilter = edgeFilters_t::SOBEL;
	}

	~sobelSettings_t(){};
};

class edgeDetectionScene : public texturedScene
{
public:
	//this was never finshed so I'm going to leave it for last
	edgeDetectionScene(texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (edge detection)",
		camera* edgeCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/EdgeDetaction.txt")
		: texturedScene(defaultTexture, windowName, edgeCamera, shaderConfigPath)
	{

	}

	/*void InitTweakBar() override
	{
		scene::InitTweakBar();
	}*/

protected:

	static edgeFilters_t		currentEdgeFilter;
};



#endif