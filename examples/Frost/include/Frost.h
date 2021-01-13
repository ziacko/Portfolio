#ifndef FROST_H
#define FROST_H
#include "../../HeatHaze/include/HeatHaze.h"

class frostScene : public heatHazeScene
{
public:

	frostScene(
		bufferHandler_t<bubbleSettings_t> bubbleSettings = bufferHandler_t<bubbleSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio ( frost )",		
		camera* bubbleCamera = new camera(), 
		const char* shaderConfigPath = "../../resources/shaders/HeatHaze.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f) : heatHazeScene(bubbleSettings, defaultTexture, windowName, bubbleCamera, shaderConfigPath)
	{
		perlinSettings_t perl;
		perl.numOctaves = 5;
		
		perl.uvScale = glm::vec2(10, 10);
		perl.taylorInverse = 0.175;
		perl.colorBias = 1.0f;
		perl.patternValue1 = 1.075;
		perl.patternValue2 = 1.257;
		perlin.data = perl;

		bubble.data.offset = 0.015f;
	}

	~frostScene( void ){}


protected:

};

#endif
