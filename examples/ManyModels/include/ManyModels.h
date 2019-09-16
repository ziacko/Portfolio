#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H
#include "Scene3D.h"

class texturedModel : public scene3D
{
public:

	texturedModel(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 1.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/texturedModel.txt",
		model_t* model = new model_t("../../resources/models/SoulSpear/SoulSpear.fbx"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{

	}

	~texturedModel(){};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		//load each texture. TODO: improve texture system to make sure textures aren't reloaded
		/*for (auto iter : testModel->meshes)
		{
			for (auto tex : iter.textures)
			{
				tex.LoadTexture();
				std::string name = tex.GetUniformName();

				int nameLength = name.length();
				char* charArray = nullptr;
				charArray = (char*)malloc(name.length() + 1);
				strcpy(charArray, name.c_str());

				textureNames.push_back(static_cast<const char*>(charArray));
			}
		}*/
	}


protected:

	int currentTexture = 0;
	std::vector<const char*> textureNames;

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene::BuildGUI(window, io);
		ImGui::Begin("textures");
		ImGui::ListBox("loaded textures", &currentTexture, textureNames.data(), textureNames.size());
		ImGui::End();

		scene3D::ImGuiCameraSettings();
	}

	virtual void Draw()
	{
		DrawMeshes();
	
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
};

#endif