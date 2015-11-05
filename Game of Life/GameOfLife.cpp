#pragma once

#include <map>
#include <stdlib.h>
#include <TinyExtender.h>
#include <TinyShaders.h>
#include <TinyWindow.h>
#include <Camera.h>
#include <DefaultUniformBuffer.h>

const GLchar* WindowName = "Ziyad Barakat's Portfolio ( Game of Life )";
GLuint vertexArrayObject = 0;
GLuint vertexBufferObject = 0;
GLuint indexBufferObject = 0;

glm::mat4 projection = glm::ortho( 0, 1280, 0, 720, 0, 10 );
glm::mat4 position = glm::mat4( 1 );
glm::mat4 view = glm::mat4( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, -5, 1 );

GLfloat quadVerts[ ] = 
{
	0.0f, 0.0f, 0.1f,
   	1280.0f, 0.01, 0.1f,
	1280.0f, 720.0f, 0.1f,
	0.0f, 720.0f, 0.1f	
};

enum cellState_t
{
	EMPTY = 0,
	ALIVE,
	DEAD
};

struct GOL_SettingsBuffer
{
	glm::vec4 aliveColor;
	glm::vec4 deadColor;
	glm::vec4 emptyColor;
	GLuint dimensions;


	gOLSettingsBuffer(GLuint dimensions,
		glm::vec4 aliveColor = glm::vec4(0, 1, 0, 1), 
		glm::vec4 deadColor = glm::vec4(1, 0, 0, 1),
		glm::vec4 emptyColor = glm::vec4(0, 0, 1, 1))
	{
		this->dimensions = dimensions;
		this->aliveColor = aliveColor;
		this->deadColor = deadColor;
		this->emptyColor = emptyColor;
	}	
};

class GOL_manager
{
public:

	GLfloat 						tickDelay;
	GLfloat 						currentTickDelay;
	GLuint 							randomSeed;

	GLuint 							programGLID;
	GLuint 							defaultSettingsGLID;
	GLuint 							GOLsettingsGLID;
	GLuint 							GOLstatusGLID;
	
	GLuint 							vertexBuffer;
	GLuint 							vertexArray;
	GLuint 							DefaultUniformBufferGLID;
	GLuint							GOLuniformBufferGLID;
	GLuint							GOLstatusBufferGLID;

	defaultUniformBuffer_t* 		defaultUniforms;
	GOL_SettingsBuffer* 			GOLsettingsUniforms;

	std::vector< cellState_t > 		cells;

	GOL_manager( camera* GOL_camera, GLuint program, GLuint dimensions = 100, GLfloat tickDelay = 0.0f,
			GLuint randomSeed = 1337 /* lol */, GLuint probability = 90 /* 100 */ )	
	{
		this->programGLID = program;
		this->defaultUniforms = new defaultUniformBuffer_t( GOL_camera->projection, 
				GOL_camera->view, GOL_camera->translation, GOL_camera->resolution );
	}
};

int main()
{

	return 0;
};
