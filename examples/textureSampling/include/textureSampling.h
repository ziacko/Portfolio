#ifndef TEXTURE_SAMPLING_H
#define TEXTURE_SAMPLING_H
#include <TexturedScene.h>

using texFilters_t = enum {SBHQ = 0, INTEGER, PREWITT, FREI_CHEN};

struct SBHQSettings_t
{
	float scale;

	SBHQSettings_t()
	{
		scale = 0.5f;
	}

	~SBHQSettings_t() {};
};

struct integerSettings_t
{
	glm::vec2 inScale;
	glm::vec2 outScale;

	integerSettings_t()
	{
		inScale = glm::vec2(1);
		outScale = glm::vec2(1);
	}

	~integerSettings_t() {};

};

class textureSamplingScene : public texturedScene
{
public:
	//this was never finished so I'm going to leave it for last
	textureSamplingScene(texFilters_t edgeFilter = SBHQ,
		texture* defaultTexture = new texture("../../resources/textures/concrete_1m.tga"),
		const char* windowName = "Ziyad Barakat's Portfolio (texture sampling)",
		camera* edgeCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/textureSampler.txt")
		: texturedScene(defaultTexture, windowName, edgeCamera, shaderConfigPath)
	{
		this->currentTexSampler = edgeFilter;
		SBHQUniforms = bufferHandler_t<SBHQSettings_t>();
		integerUniforms = bufferHandler_t<integerSettings_t>();
		
		//don't bother with the switch. just create them all at once. only 4 types


	}

	void Initialize() override
	{
		//defaultTexture->texDesc.minFilterSetting = GL_NEAREST;
		//defaultTexture->texDesc.magFilterSetting = GL_NEAREST;
		texturedScene::Initialize();

		integerProgramGLID = shaderPrograms[1]->handle;
		prewittProgramGLID = shaderPrograms[2]->handle;
		freiChenProgramGLID = shaderPrograms[3]->handle;

		integerUniforms.data.inScale = glm::vec2(defaultTexture->texDesc.dimensions.x, defaultTexture->texDesc.dimensions.y);
	}

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedScene::BuildGUI(window, io);

		ImGui::ListBox("edge detection types", &currentTexSampler, samplingSettings.data(), samplingSettings.size());
		switch (currentTexSampler)
		{
		case SBHQ:
		{
			BuildSBHQ_GUI();
			break;
		}

		case INTEGER:
		{
			BuildIntegerGUI();
			break;
		}

		case PREWITT:
		{
			BuildPrewittGUI();
			break;
		}

		case FREI_CHEN:
		{
			BuildFreiChen();
			break;
		}

		default:
			break;
		}
	}

	void BuildSBHQ_GUI()
	{
		ImGui::Begin("superbible HQ settings");
		ImGui::SliderFloat("scale", &SBHQUniforms.data.scale, 0.0f, 100.0f);
		ImGui::End();
	}

	void BuildIntegerGUI()
	{
		ImGui::Begin("integer scaling settings");
		ImGui::DragFloat2("input scale", &integerUniforms.data.inScale.x, 1.0f, 0.1f, 256.0f);
		ImGui::DragFloat2("output scale", &integerUniforms.data.outScale.x, 1.0f, 0.1f, 1000.0f);
		ImGui::End();
	}

	void BuildPrewittGUI()
	{

	}

	void BuildFreiChen()
	{

	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		SBHQUniforms.Initialize(1);
		integerUniforms.Initialize(2);
	}

	void Update() override
	{
		scene::Update();
		switch (currentTexSampler)
		{
		case SBHQ:
		{
			glUseProgram(programGLID);
			SBHQUniforms.Update();
			break;
		}

		case INTEGER:
		{
			glUseProgram(integerProgramGLID);
			integerUniforms.Update();
			break;
		}

		case PREWITT:
		{
			glUseProgram(prewittProgramGLID);
			//prewittBuffer.Update(gl_uniform_buffer, gl_dynamic_draw);
			break;
		}

		case FREI_CHEN:
		{
			glUseProgram(freiChenProgramGLID);
			//freiChenBuffer.Update(gl_uniform_buffer, gl_dynamic_draw);
			break;
		}
		}
	}

	void Draw() override //gotta love C++ :)
	{
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		switch (currentTexSampler)
		{
		case SBHQ:
		{
			glUseProgram(this->programGLID);
			break;
		}

		case INTEGER:
		{
			glUseProgram(this->integerProgramGLID);
			break;
		}

		case PREWITT:
		{
			glUseProgram(this->prewittProgramGLID);
			break;
		}

		case FREI_CHEN:
		{
			glUseProgram(this->freiChenProgramGLID);
			break;
		}
		default:
			break;
		}
		defaultTexture->GetUniformLocation(this->programGLID);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		DrawGUI(windows[0]);
	}

protected:

	bufferHandler_t<SBHQSettings_t> SBHQUniforms;
	bufferHandler_t<integerSettings_t> integerUniforms;


	std::vector<const char*>		samplingSettings = { "superbible HQ", "integer", "prewitt", "frei chen" };
	int								currentTexSampler;
	GLuint							integerProgramGLID;
	GLuint							prewittProgramGLID;
	GLuint							freiChenProgramGLID;

};
#endif