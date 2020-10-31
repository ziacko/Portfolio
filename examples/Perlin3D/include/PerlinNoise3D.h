#ifndef PERLINNOISE3D_H
#define PERLINNOISE3D_H
#include "../../perlinNoise/include/PerlinNoise.h"
#include "FrameBuffer.h"

struct perlinSettings3D_t
{
	glm::vec4		input;

	perlinSettings3D_t(glm::vec4 input = glm::vec4(1))
	{
		this->input = input;
	}

	~perlinSettings3D_t() {};
};

class perlinScene3D : public perlinScene
{
public:

	perlinScene3D(const char* windowName = "Ziyad Barakat's Portfolio ( Perlin3D noise )",
		camera* perlinCamera = new camera(), 
		const GLchar* shaderConfigPath = "../../resources/shaders/Perlin3D.txt")
		: perlinScene(windowName, perlinCamera, shaderConfigPath)
	{
		perlinBuffer = new frameBuffer();
		perlin.data = perlinSettings_t();
		perlin3D.data = perlinSettings3D_t();
		tweakBarName = windowName;
		
	}

	void Initialize() override
	{
		scene::Initialize();

		perlinBuffer->Initialize();
		perlinBuffer->Bind();

		FBODescriptor perlinDesc;
		perlinDesc.target = gl_texture_2d_array;
		perlinDesc.layers = 8;
		perlinDesc.dataType = GL_FLOAT;
		perlinDesc.format = GL_RED;
		perlinDesc.internalFormat = gl_r16;

		perlinBuffer->AddAttachment(new frameBuffer::attachment_t("perlin",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height), perlinDesc));

		frameBuffer::Unbind();

		finalProgram = shaderPrograms[1]->handle;

		scene::InitializeUniforms();
		perlin3D.Initialize(1);
	}

protected:

	unsigned int							finalProgram;
	frameBuffer*							perlinBuffer;
	bufferHandler_t<perlinSettings3D_t>		perlin3D;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		perlinScene::BuildGUI(window, io); 
		ImGui::SliderFloat4("vars", &perlin3D.data.input[0], 0, 10);
	}

	void AddGUISpacer()
	{
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Spacing();
	}

	void InitializeUniforms() override
	{
		scene::InitializeUniforms();
		perlin3D.Initialize(1);
		perlin.Initialize(2);
	}

	void SetPerlinUniforms()
	{
		perlin3D.SetupUniforms(this->programGLID, "perlin3DSettings", 1);
		perlin3D.SetupUniforms(this->programGLID, "perlinSettings", 2);
	}

	void Update() override
	{
		scene::Update();
		perlin3D.Update();
		perlin.Update();
	}

	void PerlinPass()
	{
		perlinBuffer->Bind();

		GLenum drawBuffers[8] =
		{
			perlinBuffer->attachments[0]->FBODesc.format, perlinBuffer->attachments[0]->FBODesc.format,
			perlinBuffer->attachments[0]->FBODesc.format, perlinBuffer->attachments[0]->FBODesc.format,
			perlinBuffer->attachments[0]->FBODesc.format, perlinBuffer->attachments[0]->FBODesc.format,
			perlinBuffer->attachments[0]->FBODesc.format, perlinBuffer->attachments[0]->FBODesc.format
		};

		glDrawBuffers(8, &drawBuffers[0]);

		glUseProgram(this->programGLID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		perlinBuffer->Unbind();
	}

	void FinalPass()
	{
		perlinBuffer->attachments[0]->SetActive(0);
		glUseProgram(this->finalProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArraysInstanced(GL_TRIANGLES, 0, 6, (GLsizei)(bubble.data.gridDimensions * bubble.data.gridDimensions));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

	void Draw()	override
	{
		PerlinPass();
		FinalPass();

		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
#endif
