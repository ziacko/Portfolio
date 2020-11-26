#ifndef OUTLINE_H
#define OUTLINE_H
#include "../../Stencil/include/Stencil.h"
#include "FrameBuffer.h"

struct outlineSettings_t
{
	float uvScale;

	outlineSettings_t(float uvScale = 1.005f)
	{
		this->uvScale = uvScale;
	}

	~outlineSettings_t() {};
};

class outline : public stencil
{
public:

	outline(const char* windowName = "Ziyad Barakat's Portfolio(outline)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 10.0f, camera::projection_t::perspective),
		model_t* model = new model_t("../../resources/models/fbx_foliage/broadleaf_field/Broadleaf_Desktop_Field.FBX"),
		const char* shaderConfigPath = "../../resources/shaders/Outline.txt") : 
		stencil(windowName, camera3D, model, shaderConfigPath)
	{

	}

	~outline() {};

protected:

	//bufferHandler_t<outlineSettings_t> outlineBuffer;

	virtual void InitializeUniforms() override
	{
		stencil::InitializeUniforms();
		//outlineBuffer.Initialize(1);
	}

	virtual void Update() override
	{
		stencil::Update();
		//outlineBuffer.Update();
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		//ImGui::SliderFloat("outline scale", &outlineBuffer.data.uvScale, 1.0f, 1.0075f, "%.5f");
	}

	virtual void StencilPass()
	{
		geometryBuffer->Bind();
		//enable stencils, 
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0x01);
		glStencilFunc(GL_ALWAYS, 5, 0x01);
		//glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 1, 0xff);		

		geometryBuffer->attachments[1]->Draw();

		//we just need the first LOd so only do the first 3 meshes
		for (size_t iter = 0; iter < 3; iter++)
		{
			if (testModel->meshes[iter].isCollision)
			{
				continue;
			}

			testModel->meshes[iter].textures[0].SetActive(0);
			//add the previous depth?

			glBindVertexArray(testModel->meshes[iter].vertexArrayHandle);
			glUseProgram(DepthStencilProgram);
			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, testModel->meshes[iter].indices.size(), GL_UNSIGNED_INT, 0);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		geometryBuffer->Unbind();
	}

};

#endif

