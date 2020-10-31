#ifndef HEIGHT_FOG_H
#define HEIGHT_FOG_H
#include "../../Displacement/include/Displacement.h"
#include "Grid.h"

struct fogSettings_t
{
	//color
	//extinction
	glm::vec4 color;
	glm::vec4 des;
	glm::vec4 dis;	

	fogSettings_t()
	{
		this->color = glm::vec4(0);
		this->des = glm::vec4(0);
		this->dis = glm::vec4(0);
	}

	~fogSettings_t() {};

};

class heightFog : public displacement
{
public:

	heightFog(
		const char* windowName = "Ziyad Barakat's portfolio (height fog)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 0.1f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/HeightFog.txt",
		model_t* model = new model_t("../../resources/models/Blaze/Blaze.fbx"))
		: displacement(windowName, texModelCamera, shaderConfigPath, model)
	{
		diffuseMap = new texture("../../resources/textures/rock_diffuse.tga");
		displacementMap = new texture("../../resources/textures/rock_offset.tga");
	}

	~heightFog(){};

	virtual void Initialize() override
	{
		texturedModel::Initialize();
		displacementBuffer.Initialize(1);

		renderGrid = new grid(glm::ivec2(10));

		diffuseMap->LoadTexture();
		displacementMap->LoadTexture();

		fog.data.color = glm::vec4(1, 0, 0, 1);
		fog.data.des = glm::vec4(0.025, 6, 10, 0);
		fog.data.dis = glm::vec4(0.045, 40, 20, 0);
	}

protected:

	//make a grid and dump it into the engine
	bufferHandler_t<fogSettings_t>		fog;

	void InitializeUniforms() override
	{
		displacement::InitializeUniforms();
		fog.Initialize(2);
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		displacement::BuildGUI(window, io);
		ImGui::Begin("fog setttings");
		ImGui::DragFloat3("de", &fog.data.des[0], 0.01);
		ImGui::DragFloat3("di", &fog.data.dis[0], 0.01);
		ImGui::ColorPicker4("color", &fog.data.color[0]);
		ImGui::End();

	}

	virtual void Update() override
	{
		displacement::Update();
		fog.Update();
	}

/*
	virtual void Draw() override
	{
		DrawMeshes();
	
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}*/

/*
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
	}*/
};

#endif