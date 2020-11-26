#ifndef PERLINNOISE3D_H
#define PERLINNOISE3D_H
#include "../../perlinNoise/include/PerlinNoise.h"
#include "FrameBuffer.h"

struct perlinSettings3D_t
{
	glm::vec3	uvwScale;
	int			layer;

	perlinSettings3D_t(glm::vec3 uvwScale = glm::vec3(1, 1, 0.01))
	{
		this->uvwScale = uvwScale;
		layer = 0;
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
		perlin.data = perlinSettings_t();
		perlin3D.data = perlinSettings3D_t();
		
	}

	void Initialize() override
	{
		scene::Initialize();

		FBODescriptor perlinDesc;
		perlinDesc.target = gl_texture_3d;
		perlinDesc.dataType = GL_FLOAT;
		perlinDesc.format = GL_RED;
		perlinDesc.internalFormat = gl_r16;
		perlinDesc.dimensions = glm::ivec3(50);

		//don't make this a rendertarget. just regular texture
		perlinTex = new frameBuffer::attachment_t("perlin", perlinDesc);

		perlinProgram = shaderPrograms[1]->handle;

		scene::InitializeUniforms();
		perlin3D.Initialize(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

protected:

	unsigned int							perlinProgram;
	frameBuffer::attachment_t*				perlinTex;
	bufferHandler_t<perlinSettings3D_t>		perlin3D;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		perlinScene::BuildGUI(window, io); 
		ImGui::SliderFloat3("uvw Scale", &perlin3D.data.uvwScale[0], 0.01f, 100);
		ImGui::SliderInt("layer", &perlin3D.data.layer, 0, 50);
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

	void PerlinCalc()
	{
		perlinTex->BindAsImage(0);
		glUseProgram(perlinProgram);
		glDispatchCompute(2, 2, 2);
	}

	void FinalPass()
	{
		perlinTex->SetActive(0);
		glUseProgram(this->programGLID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void Draw()	override
	{
		PerlinCalc();
		FinalPass();

		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};
#endif
