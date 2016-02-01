#ifndef BUBBLE_SCENE_H
#define BUBBLE_SCENE_H
#include <TexturedScene.h>
//#include <DefaultUniformBuffer.h>

struct bubbleSettings_t
{
	GLfloat			attenuation;
	GLfloat			offset;
	GLfloat			gridDimensions;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	bubbleSettings_t(GLfloat attenuation = 0.25f, GLfloat offset = 0.1f, GLfloat gridDimensions = 100)
	{
		this->attenuation = attenuation;
		this->offset = offset;
		this->gridDimensions = gridDimensions;
	}

	~bubbleSettings_t() {};
};

class bubbleScene : public texturedScene
{
public:

	bubbleScene(
		bubbleSettings_t* bubbleSettings = new bubbleSettings_t(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio ( bubble displacement )",		
		camera* bubbleCamera = new camera(), 
		const char* shaderConfigPath = "./shaders/Bubble.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f) : texturedScene(defaultTexture, windowName, bubbleCamera, shaderConfigPath)
	{

		this->bubbleSettingsBuffer = bubbleSettings;
		this->tweakBarName = "bubble scene";
		enableWireframe = true;
	}

	~bubbleScene( void ){}

protected:

	/*void CreateGrid()
	{
		GLfloat vertStepSize = defaultUniformBuffer->width / bubbleSettingsBuffer->gridDimensions;
		GLfloat horzStepSize = defaultUniformBuffer->height / bubbleSettingsBuffer->gridDimensions;

		for (GLuint rowIter = 0; rowIter < bubbleSettingsBuffer->gridDimensions; rowIter++)
		{
			for (GLuint columnIter = 0; columnIter < bubbleSettingsBuffer->gridDimensions; columnIter++)
			{
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter), (horzStepSize * rowIter), 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter) + vertStepSize, (horzStepSize * rowIter), 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter), (horzStepSize * rowIter) + horzStepSize, 1, 1));

				gridVerts.push_back(glm::vec4((vertStepSize * columnIter), (horzStepSize * rowIter) + horzStepSize, 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter) + vertStepSize, (horzStepSize * rowIter), 1, 1));
				gridVerts.push_back(glm::vec4((vertStepSize * columnIter) + vertStepSize, (horzStepSize * rowIter) + horzStepSize, 1, 1));
			}
		}
	}*/

	static bubbleSettings_t*	bubbleSettingsBuffer;
	std::vector<glm::vec4>		gridVerts;
	GLuint						gridDimensions;
	GLboolean					enableWireframe;

	void InitTweakBar() override
	{
		scene::InitTweakBar();
		TwAddVarRW(tweakBar, "attenuation", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->attenuation, "min=0.01 max=1 step=0.01");
		TwAddVarRW(tweakBar, "offset", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->offset, "min=-1 max=1 step=0.01");
		TwAddVarRW(tweakBar, "grid dimensions", TwType::TW_TYPE_FLOAT, &bubbleSettingsBuffer->gridDimensions, "min=1 max=1000");
		TwAddVarRW(tweakBar, "enable wireframe", TwType::TW_TYPE_BOOL8, &enableWireframe, NULL);
	}

	void InitializeBuffers() override
	{
		//SetupVertexBuffer();
		scene::InitializeBuffers();
		SetupUniformBuffer<bubbleSettings_t>(bubbleSettingsBuffer, bubbleSettingsBuffer->bufferHandle, 1);
	}

	void SetupVertexBuffer() override
	{
		GLfloat cellWidth = defaultUniformBuffer->resolution.x / bubbleSettingsBuffer->gridDimensions;
		GLfloat cellHeight = defaultUniformBuffer->resolution.y / bubbleSettingsBuffer->gridDimensions;

		GLfloat quadVerts[] =
		{
			0.0f, 0.0f, 0.1f, 1.0f,
			cellWidth, 0.0f, 0.1f, 1.0f,
			cellWidth, cellHeight, 0.1f, 1.0f,
			0.0f, cellHeight, 0.1f, 1.0f
		};

		glEnable(GL_DEPTH_TEST);

		glGenBuffers(1, &vertexBufferObject);
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, quadVerts, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (char*)(sizeof(float) * 4));
	}

	void SetupBubbleUniforms()
	{
		//textureUniformHandle = glGetUniformLocation(programGLID, "bubbleTexture");
		//bubbleSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "bubbleSettings");
	}

	void Draw()	override
	{
		UpdateUniformBuffer<bubbleSettings_t>(bubbleSettingsBuffer, bubbleSettingsBuffer->bufferHandle);
		defaultTexture->GetUniformLocation(programGLID);
		glUseProgram(this->programGLID);
		if (enableWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)(bubbleSettingsBuffer->gridDimensions * bubbleSettingsBuffer->gridDimensions));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		TwDraw();
		windowManager::WindowSwapBuffersByIndex(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

bubbleSettings_t* bubbleScene::bubbleSettingsBuffer = nullptr;

#endif