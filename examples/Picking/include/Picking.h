#ifndef PICKING_H
#define PICKING_H
#include "../../Outline/include/Outline.h"
#include "FrameBuffer.h"
#include "Transform.h"
#include "Ray.h"

struct pick_t
{
	glm::vec4 color;
};

struct pickMesh_t
{
	std::vector<glm::vec4>	positions;
};

struct result_t
{
	float		isHit;
	glm::vec4	point;
};

class picking : public outline
{
public:

	picking(const char* windowName = "Ziyad Barakat's Portfolio(picking)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 0.1f, camera::projection_t::perspective),
		model_t* model = new model_t("../../resources/models/SoulSpear/SoulSpear.FBX", false, true),
		const char* shaderConfigPath = "../../resources/shaders/Picking.txt") : 
		outline(windowName, camera3D, model, shaderConfigPath)
	{
		pickBuffer = new frameBuffer();
		pickProgram = 0;
	}

	~picking() {};

	void Initialize() override
	{
		scene3D::Initialize();

		FBODescriptor depthDesc;
		depthDesc.dataType = GL_FLOAT;
		depthDesc.format = GL_DEPTH_COMPONENT;
		depthDesc.internalFormat = gl_depth_component24;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depth;
		depthDesc.wrapRSetting = GL_CLAMP;
		depthDesc.wrapSSetting = GL_CLAMP;
		depthDesc.wrapTSetting = GL_CLAMP;
		depthDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		FBODescriptor stencilDesc;
		stencilDesc.dataType = gl_unsigned_int_24_8;
		stencilDesc.format = gl_depth_stencil;
		stencilDesc.internalFormat = gl_depth24_stencil8;
		stencilDesc.attachmentType = FBODescriptor::attachmentType_t::depthAndStencil;
		stencilDesc.wrapRSetting = GL_CLAMP;
		stencilDesc.wrapSSetting = GL_CLAMP;
		stencilDesc.wrapTSetting = GL_CLAMP;
		stencilDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		FBODescriptor pickDesc;
		pickDesc.dataType = GL_UNSIGNED_BYTE;
		pickDesc.format = GL_RED;
		pickDesc.internalFormat = gl_r8;
		pickDesc.attachmentType = FBODescriptor::attachmentType_t::color;
		pickDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		FBODescriptor colorDesc;
		colorDesc.dimensions = glm::ivec3(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height, 1);

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color", colorDesc));
		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth", stencilDesc));

		pickBuffer->Initialize();
		pickBuffer->Bind();

		pickBuffer->AddAttachment(new frameBuffer::attachment_t("color", pickDesc));

		frameBuffer::Unbind();

		DepthStencilProgram = shaderPrograms[1]->handle;
		compareProgram = shaderPrograms[2]->handle;
		finalProgram = shaderPrograms[3]->handle;
		pickProgram = shaderPrograms[4]->handle;
		pickCompProgram = shaderPrograms[5]->handle;

		pick.data.color = glm::vec4(1, 0, 0, 1);
		
		hitResult.data.isHit = false;
		hitResult.data.point = glm::vec4(0);
		pickMesh.data.positions = testModel->posData[0];
		pickMesh.Initialize(0, gl_shader_storage_buffer, gl_dynamic_draw);
		pickMesh.Update(gl_shader_storage_buffer, gl_dynamic_draw, sizeof(glm::vec4) * pickMesh.data.positions.size() + sizeof(float), pickMesh.data.positions.data());
	}

protected:

	frameBuffer*					pickBuffer;
	bufferHandler_t<pick_t>			pick;
	bufferHandler_t<pickMesh_t>		pickMesh;
	bufferHandler_t<result_t>		hitResult;
	unsigned int					pickProgram;
	unsigned int					pickCompProgram;
	bool							readBuffer = false;

	void BuildGUI(tWindow* window, ImGuiIO io) override
	{
		scene3D::BuildGUI(windows[0], io);
		for (auto iter : geometryBuffer->attachments)
		{
			ImGui::Image((ImTextureID*)iter->GetHandle(), ImVec2(512, 288),
				ImVec2(0, 1), ImVec2(1, 0));
			ImGui::SameLine();
			ImGui::Text("%s\n", iter->GetUniformName().c_str());
		}
		ImGui::Image((ImTextureID*)pickBuffer->attachments[0]->GetHandle(), ImVec2(512, 288),
			ImVec2(0, 1), ImVec2(1, 0));
	}

	void InitializeUniforms() override
	{
		stencil::InitializeUniforms();
		pick.Initialize(2);
		hitResult.Initialize(1, gl_shader_storage_buffer, gl_dynamic_draw);
	}

	void Update() override
	{
		stencil::Update();
		pick.Update();		
		//hitResult.Update(gl_shader_storage_buffer);
	}

	void Draw() override
	{
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		//RayCast(); //camera must be in perspective mode
		
		glEnable(GL_STENCIL_TEST);
		StencilPass();
		glDisable(GL_STENCIL_TEST);

		GeometryPass(); //render current scene with jitter
		
		glEnable(GL_SCISSOR_TEST);
		PickPass();
		glDisable(GL_SCISSOR_TEST);

		sceneCamera->ChangeProjection(camera::projection_t::orthographic);
		UpdateDefaultBuffer();

		FinalPass(geometryBuffer->attachments[0], geometryBuffer->attachments[1]);

		if (readBuffer)
		{
			pickBuffer->Bind(gl_read_framebuffer);
			glReadBuffer(gl_color_attachment0);
			
			unsigned char* pixels = new unsigned char[16 * 16];
			//just read from red buffer
			glReadPixels(windows[0]->mousePosition.x - 8, windows[0]->settings.resolution.height - (windows[0]->mousePosition.y + 8), 16, 16, GL_RED, GL_UNSIGNED_BYTE, pixels);
			
			std::vector<float> checker(pixels, pixels + (16 * 16));
			
			bool hitAnything = false;
			for(auto iter : checker)
			{
				if(iter > 0)
				{
					hitAnything = true;
					break;					
				}
			}
			testModel->isPicked = hitAnything;
			readBuffer = false;
		}

		DrawGUI(windows[0]);
		windows[0]->SwapDrawBuffers();
		ClearBuffers();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	}

	virtual void GeometryPass()
	{
		geometryBuffer->Bind();
		//enable stencils, 

		geometryBuffer->DrawAll();

		testModel->meshes[0].textures[0].SetActive(0);
		
		glBindVertexArray(testModel->meshes[0].vertexArrayHandle);
		glUseProgram(programGLID);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		geometryBuffer->Unbind();
	}

	virtual void PickPass()
	{
		//same as geometry pass but with a much smaller window?
		pickBuffer->Bind();

		pickBuffer->DrawAll();

		testModel->meshes[0].textures[0].SetActive(0);
		//add the previous depth?

		glBindVertexArray(testModel->meshes[0].vertexArrayHandle);
		glUseProgram(pickProgram);

		glScissor(windows[0]->mousePosition.x - 8, windows[0]->settings.resolution.height - (windows[0]->mousePosition.y + 8), 16, 16);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		pickBuffer->Unbind();
		
	}

	virtual void StencilPass()
	{
		geometryBuffer->Bind();
		//enable stencils, 
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
		if(testModel->isPicked)
		{
			glStencilMask(0x01);
			glStencilFunc(GL_ALWAYS, 5, 0x01);
		}

		else
		{
			//just to remove that error message for now
			glStencilMask(0);
			glStencilFunc(GL_ALWAYS, 5, 0);
		}

		geometryBuffer->attachments[1]->Draw();

		testModel->meshes[0].textures[0].SetActive(0);

		glBindVertexArray(testModel->meshes[0].vertexArrayHandle);
		glUseProgram(DepthStencilProgram);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		if (wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		glDrawElements(GL_TRIANGLES, (GLsizei)testModel->meshes[0].indices.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		geometryBuffer->Unbind();
		
	}

	virtual void FinalPass(texture* tex1, frameBuffer::attachment_t* tex2) override
	{
		//draw directly to backbuffer
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		tex2->SetReadMode(FBODescriptor::attachmentType_t::depthAndStencil);

		tex1->SetActive(0);
		tex2->SetActive(1);		
		
		glUseProgram(compareProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	virtual void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state) override
	{
		scene3D::HandleMouseClick(window, button, state);
		if (button == mouseButton_t::left && state == buttonState_t::down)
		{
			readBuffer = true;
			/*sceneCamera->ChangeProjection(camera::projection_t::perspective);
			sceneCamera->Update();

			UpdateDefaultBuffer();

			RayCast(); //camera must be in perspective mode*/
		}
	}

	void RayCast()
	{
		glUseProgram(pickCompProgram);
		glDispatchCompute(32, 1, 1);

		glFinish();
		glBindBuffer(gl_shader_storage_buffer, hitResult.bufferHandle);
		result_t result;
		glGetBufferSubData(gl_shader_storage_buffer, 0, sizeof(result), &result);
		printf("result: %f | point: %f, %f, %f \n", result.isHit, result.point.x, result.point.y, result.point.z);
		hitResult.data.isHit = false;
		hitResult.Update();
		testModel->isPicked = result.isHit;

		//raycast::ray testRay;
		//raycast::RayFromMouse(*sceneCamera, defaultPayload.data.mousePosition, *testModel, transform());

	}

	virtual void ClearBuffers()
	{
		//ok copy the current frame into the previous frame and clear the rest of the buffers	
		float clearColor1[4] = { 0.25f, 0.25f, 0.25f, 0.25f };
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		//geometryBuffer->ClearTexture(geometryBuffer->attachments[1], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		geometryBuffer->Unbind();

		pickBuffer->Bind();
		pickBuffer->ClearTexture(pickBuffer->attachments[0], clearColor2);
		glClear(GL_DEPTH_BUFFER_BIT);
		pickBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::ivec2 resolution)
	{
		stencil::ResizeBuffers(resolution);
		pickBuffer->Resize(glm::ivec3(resolution, 1));
	}
};

#endif

