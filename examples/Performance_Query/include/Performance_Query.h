#ifndef PERFORMANCE_QUERY_H
#define PERFORMANCE_QUERY_H
#include "../../texturedModel/include/texturedModel.h"
#include "GPUQuery.h"

class Performance_Query : public texturedModel
{
public:

	Performance_Query(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 1.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/texturedModel.txt",
		model_t* model = new model_t("../../resources/models/SoulSpear/SoulSpear.fbx"))
		: texturedModel(windowName, texModelCamera, shaderConfigPath, model)
	{
		geomTimer = nullptr;
	}

	~Performance_Query(){};

	virtual void Initialize() override
	{
		texturedModel::Initialize();
		geomTimer = new GPUTimer();
	}

protected:

	int currentTexture = 0;
	std::vector<const char*> textureNames;

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		texturedModel::BuildGUI(window, io);

		//ok from here make a new window for performance queries
		ImGui::Begin("Performance");
		ImGui::Text("GPU timer: Milliseconds | %f", geomTimer->GetTimeMilliseconds());
		ImGui::End();
	}

	virtual void Draw() override
	{
		geomTimer->Begin();
		DrawMeshes();
		//glFlush();
		geomTimer->End();

		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	virtual void DrawMeshes()
	{
		for (auto iter : testModel->meshes)
		{
			if (iter.isCollision)
			{
				continue;
			}
			//set the materials per mesh
			materialSettingsBuffer->diffuse = iter.diffuse;
			materialSettingsBuffer->ambient = iter.ambient;
			materialSettingsBuffer->specular = iter.specular;
			materialSettingsBuffer->reflective = iter.reflective;
			UpdateBuffer(materialSettingsBuffer, materialSettingsBuffer->bufferHandle, sizeof(*materialSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);

			//glBindBuffer(gl_element_array_buffer, iter.indexBufferHandle);
			glBindVertexArray(iter.vertexArrayHandle);
			glUseProgram(this->programGLID);

			glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);

			if (!iter.textures.empty())
			{
				iter.textures[currentTexture].SetActive();
			}

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, iter.indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	GPUTimer* geomTimer;
};

#endif