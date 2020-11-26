#include "TexturedScene.h"
//#include <DefaultUniformBuffer.h>

struct bubbleSettings_t
{
	float			attenuation;
	float			offset;
	float			gridDimensions;

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
		bufferHandler_t<bubbleSettings_t> bubbleSettings = bufferHandler_t<bubbleSettings_t>(),
		texture* defaultTexture = new texture(),
		const char* windowName = "Ziyad Barakat's Portfolio ( bubble displacement )",		
		camera* bubbleCamera = new camera(), 
		const char* shaderConfigPath = "../../resources/shaders/Bubble.txt", GLfloat attenuation = 1.0f, 
		GLfloat offset = 1.0f) : texturedScene(defaultTexture, windowName, bubbleCamera, shaderConfigPath)
	{

		this->bubble = bubbleSettings;
		enableWireframe = true;
		Initialize();
	}

	~bubbleScene( void ){}

protected:

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);
		ImGui::Checkbox("enable wireframe", &enableWireframe);
		
		ImGui::SliderFloat("Attenuation", &bubble.data.attenuation, 0.0f, 1.0f);
		ImGui::SliderFloat("grid dimensions", &bubble.data.gridDimensions, 0.0f, 1000.0f, "%.0f");
		ImGui::SliderFloat("offset", &bubble.data.offset, 0.0f, 1.0f);
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

	bufferHandler_t<bubbleSettings_t>			bubble;
	std::vector<glm::vec4>						gridVerts = {};
	int											gridDimensions = 0;
	bool										enableWireframe = false;

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		bubble.Initialize(1);
	}

	void SetupVertexBuffer() override
	{ 
		GLfloat cellWidth = defaultPayload.data.resolution.x / bubble.data.gridDimensions;
		GLfloat cellHeight = defaultPayload.data.resolution.y / bubble.data.gridDimensions;

		defaultVertexBuffer = new vertexBuffer_t(glm::vec2(cellWidth, cellHeight));
	}

	void Update() override
	{
		scene::Update();
		bubble.Update(gl_uniform_buffer, gl_dynamic_draw);
	}

	void Draw()	override
	{
		defaultTexture->GetUniformLocation(programGLID);
		glUseProgram(this->programGLID);
		if (enableWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, bubble.data.gridDimensions * bubble.data.gridDimensions);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		texturedScene::DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
