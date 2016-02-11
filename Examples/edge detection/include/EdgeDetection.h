#ifndef EDGEDETECTION_H
#define EDGEDETECTION_H
#include <TexturedScene.h>

typedef enum {SOBEL = 0, LAPLACIAN, PREWITT, FREI_CHEN} edgeFilters_t;

struct sobelSettings_t
{
	GLuint			bufferHandle;
	GLuint			uniformHandle;

	sobelSettings_t()
	{

	}

	~sobelSettings_t(){};
};

class edgeDetectionScene : public texturedScene
{
public:

	edgeDetectionScene(texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio (edge detection)",
		camera* edgeCamera = new camera(),
		const char* shaderConfigPath = "./shaders/EdgeDetaction.txt")
		: texturedScene(defaultTexture, windowName, edgeCamera, shaderConfigPath)
	{

	}

	void InitTweakBar() override
	{
		scene::InitTweakBar();
	}

protected:

	static edgeFilters_t		currentEdgeFilter;
};



#endif