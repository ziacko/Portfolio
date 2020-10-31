#ifndef DISPLACEMENT_H
#define DISPLACEMENT_H
#include "../../texturedModel/include/texturedModel.h"
#include "Grid.h"

struct displacementSettings_t
{
	float outerTessLevel;
	float innerTessLevel;
	float offsetStrength;

	displacementSettings_t(float outerTessLevel = 10.0f, float innerTessLevel = 1.0f, float offsetStrength = 1.0f)
	{
		this->outerTessLevel = outerTessLevel;
		this->innerTessLevel = innerTessLevel;
		this->offsetStrength = offsetStrength;
	}

	~displacementSettings_t() {};
};

class displacement : public texturedModel
{
public:

	displacement(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 0.1f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/Displacement.txt",
		model_t* model = new model_t("../../resources/models/Blaze/Blaze.fbx"))
		: texturedModel(windowName, texModelCamera, shaderConfigPath, model)
	{
		diffuseMap = new texture("../../resources/textures/rock_diffuse.tga");
		displacementMap = new texture("../../resources/textures/rock_offset.tga");
	}

	~displacement(){};

	virtual void Initialize() override
	{
		texturedModel::Initialize();
		displacementBuffer.Initialize(1);

		renderGrid = new grid(glm::ivec2(10));

		diffuseMap->LoadTexture();
		displacementMap->LoadTexture();
	}

protected:

	//make a grid and dump it into the engine

	int currentTexture = 0;
	std::vector<const char*> textureNames;
	grid* renderGrid;
	texture* displacementMap;
	texture* diffuseMap;
	bufferHandler_t<displacementSettings_t> displacementBuffer;

	void InitializeUniforms() override
	{
		texturedModel::InitializeUniforms();
		displacementBuffer.Initialize(1);
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(window, io);
		ImGui::Begin("textures");
		//ImGui::ListBox("loaded textures", &currentTexture, textureNames.data(), textureNames.size());
		ImGui::SliderFloat("outer tessellation", &displacementBuffer.data.outerTessLevel, 1.0f, 100.0f);
		ImGui::SliderFloat("inner tessellation", &displacementBuffer.data.innerTessLevel, 1.0f, 100.0f);
		ImGui::SliderFloat("offset strength", &displacementBuffer.data.offsetStrength, 1.0f, 10.0f);
		ImGui::Image((ImTextureID*)diffuseMap->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID*)displacementMap->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}

	virtual void Update() override
	{
		texturedModel::Update();
		displacementBuffer.Update();
	}

	virtual void Draw() override
	{
		DrawMeshes();
	
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void DrawMeshes()
	{
		//glBindBuffer(gl_element_array_buffer, iter.indexBufferHandle);
		glBindVertexArray(renderGrid->vertexArrayHandle);
		glUseProgram(this->programGLID);

		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		diffuseMap->SetActive(0);
		displacementMap->SetActive(1);

		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		//gotta change this to patches for tesselation
		glDrawElements(gl_patches, renderGrid->indices.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
};

#endif