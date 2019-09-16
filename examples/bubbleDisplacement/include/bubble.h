#include "TexturedScene.h"
//#include <DefaultUniformBuffer.h>

struct bubbleSettings_t
{
	float			attenuation;
	float			offset;
	float			gridDimensions;

	GLuint			bufferHandle;
	GLuint			uniformHandle;

	bubbleSettings_t(GLfloat attenuation = 0.25f, GLfloat offset = 0.1f, GLfloat gridDimensions = 100)
	{
		this->attenuation = attenuation;
		this->offset = offset;
		this->gridDimensions = gridDimensions;

		bufferHandle = 0;
		uniformHandle = 0;
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
		const char* shaderConfigPath = "../../resources/shaders/Bubble.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f) : texturedScene(defaultTexture, windowName, bubbleCamera, shaderConfigPath)
	{

		this->bubbleSettingsBuffer = bubbleSettings;
		this->tweakBarName = "bubble scene";
		enableWireframe = true;
		Initialize();
	}

	~bubbleScene( void ){}

protected:

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::Checkbox("enable wireframe", &enableWireframe);
		
		ImGui::SliderFloat("Attenuation", &bubbleSettingsBuffer->attenuation, 0.0f, 1.0f);
		ImGui::SliderFloat("grid dimensions", &bubbleSettingsBuffer->gridDimensions, 0.0f, 1000.0f, "%.0f");
		ImGui::SliderFloat("offset", &bubbleSettingsBuffer->offset, 0.0f, 1.0f);
	}

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

	bubbleSettings_t*			bubbleSettingsBuffer = nullptr;
	std::vector<glm::vec4>		gridVerts = {};
	int							gridDimensions = 0;
	bool						enableWireframe = false;

	void InitializeUniforms() override
	{
		//SetupVertexBuffer();
		scene::InitializeUniforms();
		SetupBuffer(bubbleSettingsBuffer, bubbleSettingsBuffer->bufferHandle, sizeof(*bubbleSettingsBuffer), 1, gl_uniform_buffer, gl_dynamic_draw);
	}

	void SetupVertexBuffer() override
	{ 
		GLfloat cellWidth = defaultUniform->resolution.x / bubbleSettingsBuffer->gridDimensions;
		GLfloat cellHeight = defaultUniform->resolution.y / bubbleSettingsBuffer->gridDimensions;

		defaultVertexBuffer = new vertexBuffer_t(glm::vec2(cellWidth, cellHeight));
	}

	void SetupBubbleUniforms()
	{
		//textureUniformHandle = glGetUniformLocation(programGLID, "bubbleTexture");
		//bubbleSettingsBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "bubbleSettings");
	}

	void Draw()	override
	{
		UpdateBuffer(bubbleSettingsBuffer, bubbleSettingsBuffer->bufferHandle, sizeof(*bubbleSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);
		defaultTexture->GetUniformLocation(programGLID);
		glUseProgram(this->programGLID);
		if (enableWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)(bubbleSettingsBuffer->gridDimensions * bubbleSettingsBuffer->gridDimensions));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		texturedScene::DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
