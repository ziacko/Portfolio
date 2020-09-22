#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H
#include "Scene3D.h"

class speedTree : public scene3D
{
public:

	speedTree(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 5.0f, camera::projection_t::perspective),
		const char* shaderConfigPath = "../../resources/shaders/SpeedTree.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.fbx"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	~speedTree(){};

	virtual void Initialize() override
	{
		scene3D::Initialize();
	}

protected:

	void Draw() override
	{
		//we just need the first LOD so only do the first 3
		for (size_t iter = 0; iter < 3; iter++)
		{
			if (testModel->meshes[iter].isCollision)
			{
				continue;
			}
			//set the materials per mesh
			materialBuffer.data.diffuse = testModel->meshes[iter].diffuse;
			materialBuffer.data.ambient = testModel->meshes[iter].ambient;
			materialBuffer.data.specular = testModel->meshes[iter].specular;
			materialBuffer.data.reflective = testModel->meshes[iter].reflective;

			materialBuffer.Update();
			//trial and error
			testModel->meshes[iter].textures[0].SetActive(0);

			//glBindBuffer(gl_element_array_buffer, iter.indexBufferHandle);
			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(this->programGLID);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//testModel->meshes[iter].textures[0].UnbindTexture();
		}
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

#endif