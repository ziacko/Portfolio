#ifndef TRANSFORMSCENE_H
#define TRANSFORMSCENE_H
#include "Scene3D.h"
#include "Transform.h"
#include "ImGuizmo.h"

struct transformSettings_t
{
	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 scale;
	glm::vec3 eulerAngles; //could be useful

	transformSettings_t()
	{
		position = glm::mat4(1.0f);
		rotation = glm::mat4(1.0f);
		scale = glm::mat4(1.0f);
		eulerAngles = glm::vec3(0.0f);
	}
};

class transformScene : public scene3D
{
public:

	transformScene(
		const char* windowName = "Ziyad Barakat's portfolio (textured Model)",
		camera* texModelCamera = new camera(glm::vec2(1280, 720), 1.0f, camera::projection_t::perspective, 0.1f, 1000000.f),
		const char* shaderConfigPath = "../../resources/shaders/transform.txt",
		model_t* model = new model_t("../../resources/models/Dragon.fbx"))
		: scene3D(windowName, texModelCamera, shaderConfigPath, model)
	{
		testTrans = new transform();

		transformUniforms = bufferHandler_t<transformSettings_t>();
		gizmoOp = ImGuizmo::TRANSLATE;
		gizmoMode = ImGuizmo::LOCAL;
	}

	~transformScene(){};

	bufferHandler_t<transformSettings_t> transformUniforms;

	ImGuizmo::OPERATION gizmoOp;// (ImGuizmo::ROTATE);
	ImGuizmo::MODE gizmoMode;// (ImGuizmo::LOCAL);


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

	transform* testTrans;

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		ImGui::Begin(window->settings.name, &isGUIActive);
		scene::BuildGUI(window, io);

		ImGuizmo::ViewManipulate((float*)&sceneCamera->view, 30.0f, ImVec2(window->settings.resolution.x - 128, 0), ImVec2(128, 128), 0x10101010);
		ImGui::Begin("textures");
		ImGui::ListBox("loaded textures", &currentTexture, textureNames.data(), textureNames.size());
		ImGui::End();
		
		TransformRedundancy();

		scene3D::ImGuiCameraSettings();
	}

	virtual void BuildTransformStuff(tWindow* window)
	{
		//ImGui::Begin("transform");
		ImGuizmo::SetOrthographic(false);

		ImGuizmo::BeginFrame();
		ImGuizmo::Enable(true);
		ImGuizmo::SetRect(0, 0, window->settings.resolution.x, window->settings.resolution.y);
		//are Z and Y swapped?
		glm::quat gridRotation(glm::radians(glm::vec3(90, 0, 0)));
		glm::mat4 gridMat = glm::mat4(1.0f) * glm::toMat4(gridRotation);

		glm::mat4 view = sceneCamera->view;
		glm::mat4 projection = sceneCamera->projection;

		ImGuizmo::DrawGrid((const float*)&view, (const float*)&projection, (float*)&gridMat, 10.0f);

		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		glm::mat4 objectMatrix = glm::mat4(1.0f);
		glm::mat4 position = glm::translate(glm::mat4(1.0f), glm::vec3(testTrans->position));
		glm::mat4 rotation = glm::mat4_cast(glm::quat(glm::radians(testTrans->eulerAngles)));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(testTrans->scale));

		objectMatrix = objectMatrix * position * rotation * scale;
		
		ImGuizmo::Manipulate((const float*)&view, (const float*)&projection, gizmoOp, gizmoMode, (float*)&objectMatrix);
		
		//rotation comes back in radians?
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents((float*)&objectMatrix, matrixTranslation, matrixRotation, matrixScale);

		testTrans->position = glm::vec4(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2], 0);
		testTrans->eulerAngles = glm::vec4(matrixRotation[0], matrixRotation[1], matrixRotation[2], 0);
		testTrans->rotation = glm::quat(glm::radians(testTrans->eulerAngles));
		testTrans->scale = glm::vec4(matrixScale[0], matrixScale[1], matrixScale[2], 0);
	}

	virtual void TransformRedundancy()
	{
		ImGui::Begin("transform");
		//ImGui::DragFloat3()
		ImGui::End();

	}

	virtual void Update() override
	{
		scene3D::Update();

		transformUniforms.data.position = glm::translate(glm::mat4(1.0f), glm::vec3(testTrans->position));
		transformUniforms.data.rotation = glm::toMat4(testTrans->rotation);
		transformUniforms.data.scale = glm::scale(glm::mat4(1.0f),  glm::vec3(testTrans->scale));
		transformUniforms.data.eulerAngles = glm::eulerAngles(testTrans->rotation);

		transformUniforms.Update();
	}

	virtual void BeginGUI(tWindow* window) override
	{
		ImGUINewFrame(window);
		BuildTransformStuff(window); //this needs to happen before rendering begins at least
	}

	virtual void Draw() override
	{
		BeginGUI(windows[0]);
		ImGuiIO io = ImGui::GetIO();

		DrawMeshes();
	
		BuildGUI(windows[0], io);
		EndGUI(windows[0]);

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
			materialBuffer.data.diffuse = iter.diffuse;
			materialBuffer.data.ambient = iter.ambient;
			materialBuffer.data.specular = iter.specular;
			materialBuffer.data.reflective = iter.reflective;
			materialBuffer.Update(gl_uniform_buffer, gl_dynamic_draw);

			//glBindBuffer(gl_element_array_buffer, iter.indexBufferHandle);
			glBindVertexArray(iter.vertexArrayHandle);
			glUseProgram(this->programGLID);

			glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

			if (!iter.textures.empty())
			{
				iter.textures[currentTexture].SetActive();
			}

			if (wireframe)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			glDrawElements(GL_TRIANGLES, iter.indices.size(), GL_UNSIGNED_INT, nullptr);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	virtual void InitializeUniforms() override
	{
		scene3D::InitializeUniforms();
		transformUniforms.Initialize(1);
	}

	virtual void HandleKey(tWindow* window, int key, keyState_t state) override
	{
		scene3D::HandleKey(window, key, state);

		switch(key)
		{
		case 't':
		{
			gizmoOp = ImGuizmo::TRANSLATE;
			break;
		}

		case 'r':
		{
			gizmoOp = ImGuizmo::ROTATE;
			break;
		}

		case 'y':
		{
			gizmoOp = ImGuizmo::SCALE;
			break;
		}

		case 'b':
		{
			gizmoOp = ImGuizmo::BOUNDS;
			break;
		}

		case 'l':
		{
			gizmoMode = ImGuizmo::LOCAL;
			break;
		}
		
		case 'g':
		{
			gizmoMode = ImGuizmo::WORLD;
			break;
		}
		}
	}
};

#endif