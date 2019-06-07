#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H
#include "Scene3D.h"

class speedTree : public scene3D
{
public:

	speedTree(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 500.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/SpeedTree.txt",
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.fbx"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		glEnable(GL_BLEND);
		//glEnable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	~speedTree(){};

	virtual void Initialize() override
	{
		scene3D::Initialize();

		glGenQueries(1, &defaultQuery);
	}

protected:

	std::vector<uint64_t> averageGPUTimes;
	unsigned int defaultQuery = 0;

	virtual void Draw()
	{
		glQueryCounter(defaultQuery, gl_timestamp);
		GLint64 startFrameTime = 0;
		glGetInteger64v(gl_timestamp, &startFrameTime);
		uint64_t startTime = static_cast<uint64_t>(startFrameTime);

		//we just need the first LOD so only do the first 3
		for (size_t iter = 0; iter < 3; iter++)
		{
			if (testModel->meshes[iter].isCollision)
			{
				continue;
			}
			//set the materials per mesh
			materialSettingsBuffer->diffuse = testModel->meshes[iter].diffuse;
			materialSettingsBuffer->ambient = testModel->meshes[iter].ambient;
			materialSettingsBuffer->specular = testModel->meshes[iter].specular;
			materialSettingsBuffer->reflective = testModel->meshes[iter].reflective;
			UpdateBuffer(materialSettingsBuffer, materialSettingsBuffer->bufferHandle, sizeof(*materialSettingsBuffer), gl_uniform_buffer, gl_dynamic_draw);
			//leavesDiffuse->UnbindTexture();
			//trial and error
			testModel->meshes[iter].textures[0].SetActive(0);

			//glBindBuffer(gl_element_array_buffer, iter.indexBufferHandle);
			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(this->programGLID);
			glViewport(0, 0, windows[0]->resolution.width, windows[0]->resolution.height);
			glCullFace(GL_BACK);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			//testModel->meshes[iter].textures[0].UnbindTexture();
		}

		glQueryCounter(defaultQuery, gl_timestamp);
		GLint64 geomTime = 0;
		glGetInteger64v(gl_timestamp, &geomTime);
		uint64_t GeomTimeU = static_cast<uint64_t>(geomTime);

		uint64_t averageGPUTime = 0;
		if ((defaultUniform->totalFrames % 11) == 0)
		{
			//average the whole lot and clear the vector
			uint64_t tempTime = 0;
			for (auto iter : averageGPUTimes)
			{
				tempTime += iter;
			}

			tempTime /= 10; //wee need the average GPU time over 10 frames

			printf("%f | %f \n", (float)tempTime / 10000.0f, (float)(1.0f / (tempTime / 10000000.0f)));
			averageGPUTimes.clear();
		}

		else
		{
			//
			averageGPUTimes.push_back(GeomTimeU - startTime);
		}
		DrawGUI(windows[0]);

		windows[0]->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
};

#endif