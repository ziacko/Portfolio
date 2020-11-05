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

class picking : public outline
{
public:

	picking(const char* windowName = "Ziyad Barakat's Portfolio(outline)",
		camera* camera3D = new camera(glm::vec2(1280, 720), 0.1f, camera::projection_t::perspective),
		model_t* model = new model_t("../../resources/models/SoulSpear/SoulSpear.FBX"),
		const char* shaderConfigPath = "../../resources/shaders/Picking.txt") : 
		outline(windowName, camera3D, model, shaderConfigPath)
	{
		pickBuffer = new frameBuffer();
	}

	~picking() {};

	//bufferHandler_t<outlineSettings_t> outlineBuffer;
	virtual void Initialize() override
	{
		scene3D::Initialize();

		geometryBuffer->Initialize();
		geometryBuffer->Bind();

		FBODescriptor depthDesc;
		depthDesc.dataType = gl_unsigned_int_24_8;
		depthDesc.format = gl_depth_stencil;
		depthDesc.internalFormat = gl_depth24_stencil8;
		depthDesc.attachmentType = FBODescriptor::attachmentType_t::depthAndStencil;
		depthDesc.sampleCount = 1;
		depthDesc.wrapRSetting = GL_CLAMP;
		depthDesc.wrapSSetting = GL_CLAMP;
		depthDesc.wrapTSetting = GL_CLAMP;

		FBODescriptor stencilDesc;
		stencilDesc.dataType = GL_UNSIGNED_INT;
		stencilDesc.format = GL_STENCIL_INDEX;
		stencilDesc.internalFormat = gl_stencil_index8;
		stencilDesc.attachmentType = FBODescriptor::attachmentType_t::stencil;
		stencilDesc.sampleCount = 1;

		FBODescriptor pickDesc;
		pickDesc.dataType = GL_UNSIGNED_BYTE;
		pickDesc.format = GL_RED;
		pickDesc.internalFormat = gl_r8;
		pickDesc.attachmentType = FBODescriptor::attachmentType_t::color;

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("color",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height)));

		geometryBuffer->AddAttachment(new frameBuffer::attachment_t("depth",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height), depthDesc));

		pickBuffer->Initialize();
		pickBuffer->Bind();

		pickBuffer->AddAttachment(new frameBuffer::attachment_t("color",
			glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height), pickDesc));

		frameBuffer::Unbind();

		DepthStencilProgram = shaderPrograms[1]->handle;
		compareProgram = shaderPrograms[2]->handle;
		finalProgram = shaderPrograms[3]->handle;
		pickProgram = shaderPrograms[4]->handle;

		pick.data.color = glm::vec4(1, 0, 0, 1);

		//PBO
		/*glGenBuffers(1, &PBOHandle);
		glBindBuffer(gl_pixel_pack_buffer, PBOHandle);
		glBufferData(gl_pixel_pack_buffer, totalSize, 0, gl_stream_read);

		glGenBuffers(1, &PBOHandle2);
		glBindBuffer(gl_pixel_pack_buffer, PBOHandle);
		glBufferData(gl_pixel_pack_buffer, totalSize, 0, gl_stream_read);

		glBindBuffer(gl_pixel_pack_buffer, 0);*/
	}

protected:

	frameBuffer*				pickBuffer;
	bufferHandler_t<pick_t>		pick;
	unsigned int				pickProgram;
	bool						readBuffer = false;

	virtual void BuildGUI(tWindow* window, ImGuiIO io) override
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

	virtual void InitializeUniforms() override
	{
		stencil::InitializeUniforms();
		pick.Initialize(2);
	}

	virtual void Update() override
	{
		stencil::Update();
		pick.Update();
	}

	void Draw() override
	{
		sceneCamera->ChangeProjection(camera::projection_t::perspective);
		sceneCamera->Update();

		UpdateDefaultBuffer();

		StencilPass();
		
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

		GLenum drawbuffers[1] = {
			geometryBuffer->attachments[0]->FBODesc.attachmentFormat, //depth
		};

		glDrawBuffers(1, drawbuffers);

		testModel->meshes[0].textures[0].SetActive(0);
		//add the previous depth?

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

		GLenum drawbuffers[1] = {
			pickBuffer->attachments[0]->FBODesc.attachmentFormat, //depth
		};

		glDrawBuffers(1, drawbuffers);

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
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		if(testModel->isPicked)
		{
			glStencilMask(0x01);
			glStencilFunc(GL_ALWAYS, 5, 0x01);
		}

		else
		{
			//just to remove that error message for now
			//glStencilMask(0);
			//glStencilFunc(GL_ALWAYS, 5, 0);
		}

		GLenum drawbuffers[1] = {
			geometryBuffer->attachments[1]->FBODesc.attachmentFormat, //stencil
		};

		glDrawBuffers(1, drawbuffers);

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
		tex1->SetActive(0);

		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);

		tex2->SetActive(1);
		tex2->SetReadMode(FBODescriptor::attachmentType_t::stencil);
		glUseProgram(compareProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	virtual void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state) override
	{
		scene::HandleMouseClick(window, button, state);
		if (button == mouseButton_t::left && state == buttonState_t::down)
		{
			
			transform identity = transform();
			glm::vec2 mousePosition = glm::vec2(windows[0]->mousePosition.x, windows[0]->mousePosition.y);
			//fire off a ray from the mouse position (in world space),
			//test if this ray intersects with any triangles in every mesh
			raycast::result res = raycast::RayFromMouse(*sceneCamera, mousePosition, *testModel, identity);
			printf("hit: %i\n", res);
			readBuffer = res.hit;
		}
	}

	virtual void ClearBuffers()
	{
		//ok copy the current frame into the previous frame and clear the rest of the buffers	
		float clearColor1[4] = { 0.25f, 0.25f, 0.25f, 0.25f };
		float clearColor2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		geometryBuffer->Bind();
		geometryBuffer->ClearTexture(geometryBuffer->attachments[0], clearColor1);
		geometryBuffer->ClearTexture(geometryBuffer->attachments[1], clearColor1);
		glClear(GL_DEPTH_BUFFER_BIT);
		geometryBuffer->Unbind();

		pickBuffer->Bind();
		pickBuffer->ClearTexture(pickBuffer->attachments[0], clearColor2);
		glClear(GL_DEPTH_BUFFER_BIT);
		pickBuffer->Unbind();

		sceneCamera->ChangeProjection(camera::projection_t::perspective);
	}

	virtual void ResizeBuffers(glm::vec2 resolution)
	{
		stencil::ResizeBuffers(resolution);
		pickBuffer->Resize(resolution);
	}
};

#endif

