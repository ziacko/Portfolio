#ifndef SCENE_H
#define SCENE_H
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <string>
#include <cstdlib>
#include <map>
#include <numeric>
#include <algorithm>
#include <cstddef>
#include <chrono>
#include <thread>
#include <array>
#include <list>
#include <TinyExtender.h>
using namespace TinyExtender;
#include <TinyShaders.h>
using namespace TinyShaders;
#include <TinyWindow.h>
#include <TinyClock.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <imgui.h>
using namespace TinyWindow;
using namespace std::placeholders;
#include "Camera.h"
#include "DefaultUniformBuffer.h"
#include "GPUQuery.h"
#include "Utilities.h"
#include "VertexBuffer.h"

using frameRates_t = enum {UNCAPPED = 0, THIRTY = 30, SIXTY = 60, NINETY = 90, ONETWENTY = 120, ONEFOURTYFOUR = 144};

class scene
{
public:

	scene(const char* windowName = "Ziyad Barakat's Portfolio ( Example Scene )",
		camera* bufferCamera = new camera(),
		const char* shaderConfigPath = "../../resources/shaders/Default.txt")
	{
		this->windowName = windowName;
		this->sceneCamera = bufferCamera;
		this->shaderConfigPath = shaderConfigPath;
		this->windowName = windowName;
		defaultVertexBuffer = nullptr;
		//defaultUniform = nullptr;
		imGUIFontTexture = 0;

		isFrameRateLocked = false;
		lockedFrameRate = 60;

		manager = new windowManager();
		
		windowSetting_t setting;
		setting.name = windowName;
		setting.userData = this;
		setting.resolution = vec2_t<unsigned int>(1280, 720);
		setting.SetProfile(profile_t::core);

		windows.push_back(manager->AddWindow(setting));
		
		shaderHandler = new shaderManager();
		
		InitImGUI(windows[0]);

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		
		sceneClock = new tinyClock_t();
	}

	virtual ~scene(){};

	virtual void Run()
	{
		while (!windows[0]->shouldClose)
		{
			Update();
			Draw();
		}
	}

	virtual void Initialize()
	{
		TinyExtender::InitializeExtentions();

		if (glDebugMessageCallback == nullptr)
		{
			printf("blarg \n");
		}

		glDebugMessageCallback(&OpenGLDebugCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
			GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		
		shaderHandler->LoadShaderProgramsFromConfigFile(shaderConfigPath, false, &shaderPrograms);
		this->programGLID = shaderPrograms[0]->handle;

		glUseProgram(this->programGLID);

		InitializeUniforms();
		SetupCallbacks();
		//defaultPayload = bufferPayload<defaultUniformBuffer>();
		defaultTimer = new GPUTimer();
	}

	virtual void SetupCallbacks()
	{
		manager->resizeEvent = std::bind(&scene::HandleWindowResize, this, _1, _2);
		manager->mouseButtonEvent = std::bind(&scene::HandleMouseClick, this, _1, _2, _3);
		manager->mouseMoveEvent = std::bind(&scene::HandleMouseMotion, this, _1, _2, _3);
		manager->mouseWheelEvent = std::bind(&scene::HandleMouseWheel, this, _1, _2);
		manager->maximizedEvent = std::bind(&scene::HandleMaximize, this, _1);
		manager->keyEvent = std::bind(&scene::HandleKey, this, _1, _2, _3);
		//manager->destroyedEvent = std::bind(&scene::ShutDown, this, _1);
		manager->fileDropEvent = std::bind(&scene::HandleFileDrop, this, _1, _2, _3);
	}

	void ShutDown(tWindow* window)
	{
		//scene* thisScene = (scene*)window->userData;
		ImGUIInvalidateDeviceObject();
		shaderHandler->Shutdown();
		manager->ShutDown();
	}
	
protected:

	windowManager*							manager;

	std::map<tWindow*, ImGuiContext*>		windowContextMap;
	std::vector<tWindow*>					windows;

	shaderManager*							shaderHandler;
	std::vector<tShaderProgram*>			shaderPrograms;

	tinyClock_t*							sceneClock;

	//defaultUniformBuffer*		defaultUniform;
	vertexBuffer_t*							defaultVertexBuffer;

	bufferHandler_t<defaultUniformBuffer>	defaultPayload;

	camera*						sceneCamera;
	const char*					windowName;
	GLuint						programGLID;
	const char*					shaderConfigPath;

	ImGuiContext*				imGUIContext;
	GLuint						imGUIFontTexture;
	GLint						imGUIShaderhandle;
	GLint						imGUIVertexHandle;
	GLint						imGUIFragmentHandle;
	GLint						imGUITexAttribLocation;
	GLint						imGUIProjMatrixAttribLocation;
	GLint						imGUIPositionAttribLocation;
	GLint						imGUIUVAttribLocation;
	GLint						imGUIColorAttribLocation;
	GLuint						imGUIVBOHandle;
	GLuint						imGUIVAOHandle;
	GLuint						imGUIIBOHandle;

	bool						isGUIActive;

	bool						isFrameRateLocked;
	int							lockedFrameRate = UNCAPPED;
	std::vector<const char*>	frameRateSettings = { "none", "30", "60", "90", "120", "144" };

	int				currentResolution = 0;	

	GPUTimer*					defaultTimer;

	virtual void Update()
	{
		manager->PollForEvents();
		sceneCamera->Update();
		if (lockedFrameRate > 0)
		{
			sceneClock->UpdateClockFixed(lockedFrameRate);
		}
		else
		{
			sceneClock->UpdateClockAdaptive();
		}		

		defaultPayload.data.deltaTime = 

		defaultPayload.data.deltaTime = (float)sceneClock->GetDeltaTime();
		defaultPayload.data.totalTime = (float)sceneClock->GetTotalTime();
		defaultPayload.data.framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		defaultPayload.data.projection = sceneCamera->projection;
		defaultPayload.data.view = sceneCamera->view;
		defaultPayload.data.translation = sceneCamera->translation;

		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
		//UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void Draw()
	{
		for (auto windowIter : windows)
		{
			windowIter->MakeCurrentContext();
			//ImGui::SetCurrentContext(windowContextMap[windowIter]);
			glUseProgram(this->programGLID);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			//glDrawElements(GL_TRIANGLES, sizeof(unsigned int) * 6, GL_UNSIGNED_INT, 0);

			glViewport(0, 0, windowIter->settings.resolution.width, windowIter->settings.resolution.height);
			DrawGUI(windowIter);

			windowIter->SwapDrawBuffers();
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	virtual void BuildGUI(tWindow* window, ImGuiIO io)
	{
		ImGui::Text("FPS %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, 1.0f / sceneClock->GetDeltaTime());
		ImGui::Text("Total running time %.5f", sceneClock->GetTotalTime());
		ImGui::Text("Mouse coordinates: \t X: %.0f \t Y: %.0f", io.MousePos.x, io.MousePos.y);
		ImGui::Text("Window size: \t Width: %i \t Height: %i", window->settings.resolution.width, window->settings.resolution.height);

		if(ImGui::Button("Toggle Fullscreen"))
		{
			window->SetStyle(style_t::popup);
			window->SetPosition(vec2_t<int>::Zero());
			window->SetWindowSize(vec2_t<unsigned int>(manager->GetMonitors().back()->resolution.width, manager->GetMonitors().back()->resolution.height));
			window->ToggleFullscreen(manager->GetMonitors()[0], 0);
		}

		static int interval = 1;
		if (ImGui::SliderInt("Swap Interval", &interval, 0, 5))
		{
			manager->SetWindowSwapInterval(window, interval);
		}

		static int frameRatePick = 0;
		ImGui::ListBox("Frame rate cap", &frameRatePick, frameRateSettings.data(), (int)frameRateSettings.size());
		switch (frameRatePick)
		{
		case 0: //none
		case 1: //30
		case 2: //60
		case 3: //90
		case 4: //120
		{
			lockedFrameRate = frameRatePick * 30;
			break;
		}
		case 5: //144
		{
			lockedFrameRate = 144;
			break;
		}
		}


		/*std::vector<const char*> resolutions;
		for (int resolutionIter = 0; resolutionIter < manager->GetMonitors()[0]->settings.size(); resolutionIter += 6)
		{
			std::string* currentResolution = new std::string();
			//ok add the width, an x and then the height
			*currentResolution += std::to_string(manager->GetMonitors()[0]->settings[resolutionIter]->resolution.width);
			*currentResolution += " x ";
			*currentResolution += std::to_string(manager->GetMonitors()[0]->settings[resolutionIter]->resolution.height);
			//currentResolution += " ";
			const char* blarg = currentResolution->c_str();
			resolutions.push_back(std::move(blarg));
		}*/

		//make a combo that goes through each resolution in the list
		//if one of them are clicked on 
		/*if (ImGui::ListBox("resolutions", &currentResolution, resolutions.data(), resolutions.size()))
		{
			window->SetResolution(vec2_t<unsigned int>(manager->GetMonitors()[0]->settings[currentResolution]->resolution.width,
				manager->GetMonitors()[0]->settings[currentResolution]->resolution.height));
		}*/
		
		sceneCamera->resolution = glm::vec2(window->settings.resolution.width, window->settings.resolution.height);
		//create a separate window that displays all possible rendering resolutions
		//DrawCameraStats();
	}

	virtual void DrawCameraStats()
	{
		//set up the view matrix
		ImGui::Begin("camera", &isGUIActive);// , ImVec2(0, 0));

		ImGui::Combo("projection type", &(int)sceneCamera->currentProjectionType, "perspective\0orthographic");

		if (sceneCamera->currentProjectionType == camera::projection_t::orthographic)
		{
			ImGui::DragFloat("near plane", &sceneCamera->nearPlane);
			ImGui::DragFloat("far plane", &sceneCamera->farPlane);
			ImGui::SliderFloat("Field of view", &sceneCamera->fieldOfView, 0, 90, "%.10f");
		}

		else
		{
			ImGui::InputFloat("camera speed", &sceneCamera->speed, 0.f);
			ImGui::InputFloat("x sensitivity", &sceneCamera->xSensitivity, 0.f);
			ImGui::InputFloat("y sensitivity", &sceneCamera->ySensitivity, 0.f);
		}

		if (ImGui::TreeNode("view matrix"))
		{
			if (ImGui::TreeNode("right"))
			{
				//ImGui::Columns(2);
				//ImGui::ListBox("blarg", 0, )
				//ImGui::Text("blarg");
				//ImGui::SameLine();
				//ImGui::NextColumn();
				ImGui::DragFloat4("##", (float*)&sceneCamera->view[0], 0.1f, -100.0f, 100.0f);
				//ImGui::Columns(1);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("up"))
			{
				ImGui::DragFloat4("##", (float*)&sceneCamera->view[1], 0.1f, -100.0f, 100.0f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("forward"))
			{
				ImGui::DragFloat4("##", (float*)&sceneCamera->view[2], 0.1f, -100.0f, 100.0f);
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("position"))
			{
				ImGui::DragFloat4("##", (float*)&sceneCamera->view[3], 0.1f, -100.0f, 100.0f);
				ImGui::TreePop();
			}
			ImGui::TreePop();

		}
		if (ImGui::CollapsingHeader("projection matrix", NULL))
		{
			ImGui::DragFloat4("projection 0", (float*)&sceneCamera->projection[0], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("projection 1", (float*)&sceneCamera->projection[1], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("projection 2", (float*)&sceneCamera->projection[2], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("projection 3", (float*)&sceneCamera->projection[3], 0.1f, -100.0f, 100.0f);
		}
		if (ImGui::CollapsingHeader("translation matrix", NULL))
		{
			ImGui::DragFloat4("row 0", (float*)&sceneCamera->translation[0], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("row 1", (float*)&sceneCamera->translation[1], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("row 2", (float*)&sceneCamera->translation[2], 0.1f, -100.0f, 100.0f);
			ImGui::DragFloat4("row 3", (float*)&sceneCamera->translation[3], 0.1f, -100.0f, 100.0f);
		}
		ImGui::End();

		defaultPayload.Update(gl_uniform_buffer, gl_dynamic_draw);
		//UpdateBuffer(d, defaultUniform->bufferHandle, sizeof(defaultUniform), gl_uniform_buffer, gl_dynamic_draw);
	}

	virtual void BeginGUI(tWindow* window)
	{
		ImGUINewFrame(window);
		ImGui::Begin(window->settings.name, &isGUIActive);// , beginSize);

	}

	virtual void EndGUI(tWindow* window)
	{
		ImGui::End();
		ImGui::Render();
		HandleImGUIRender(window);
	}

	virtual void DrawGUI(tWindow* window, ImVec2 beginSize = ImVec2(0, 0))
	{
		BeginGUI(window);
		ImGuiIO io = ImGui::GetIO();
		BuildGUI(window, io);
		EndGUI(window);
	}

	virtual void SetupVertexBuffer()
	{
		defaultVertexBuffer = new vertexBuffer_t(defaultPayload.data.resolution);

		GLfloat quadVerts[24] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, 0.0f, 1.0f, 1.0f,
			0.0f, sceneCamera->resolution.y, 1.0f, 1.0f,

			sceneCamera->resolution.x, 0.0f, 1.0f, 1.0f,
			0.0f, sceneCamera->resolution.y, 1.0f, 1.0f,
			sceneCamera->resolution.x, sceneCamera->resolution.y, 1.0f, 1.0f,
		};
	}

	virtual void SetupIndexBuffer()
	{

	}

	void SetupBuffer(GLenum target, GLenum usage)
	{
		//TODO: get the currently bound buffer and rebind to that after this operation is done
		defaultPayload.Initialize(0, target, usage);
		/*glGenBuffers(1, &bufferHandle);
		UpdateBuffer(buffer, bufferHandle, bufferSize, target, usage);
		glBindBufferBase(target, bufferUniformHandle, bufferHandle);*/
	}

	//fuh. ill do it AFTER i've fixed GOL
	void UpdateBuffer(GLenum target, GLenum usage)
	{
		//TODO: get the currently bound buffer and rebind to that after this operation is done
		defaultPayload.Update(target, usage);
		/*glBindBuffer(target, bufferHandle);
		glBufferData(target, bufferSize, buffer, usage);*/
	}

	virtual void InitializeUniforms()
	{
		defaultPayload.data = defaultUniformBuffer(this->sceneCamera);
		glViewport(0, 0, windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		defaultPayload.data.resolution = glm::vec2(windows[0]->settings.resolution.width, windows[0]->settings.resolution.height);
		defaultPayload.data.projection = glm::ortho(0.0f, (GLfloat)windows[0]->settings.resolution.width, (GLfloat)windows[0]->settings.resolution.height, 0.0f, 0.01f, 10.0f);

		SetupVertexBuffer();
		SetupBuffer(gl_uniform_buffer, gl_dynamic_draw);

		SetupDefaultUniforms();
	}

	void SetupDefaultUniforms()
	{
		defaultPayload.SetupUniforms(programGLID, "defaultSettings", 0);
		/*defaultUniform->uniformHandle = glGetUniformBlockIndex(this->programGLID, "defaultSettings");
		glUniformBlockBinding(this->programGLID, defaultUniform->uniformHandle, 0);*/
	}

	virtual void Resize(tWindow* window, glm::ivec2 dimensions = glm::ivec2(0))
	{
		if (dimensions == glm::ivec2(0))
		{
			dimensions = glm::ivec2(window->settings.resolution.width, window->settings.resolution.height);
		}
		glViewport(0, 0, dimensions.x, dimensions.y);
		
		defaultPayload.data.resolution = glm::ivec2(dimensions.x, dimensions.y);
		defaultPayload.data.projection = glm::ortho(0.0f, (GLfloat)dimensions.x, (GLfloat)dimensions.y, 0.0f, 0.01f, 10.0f);

		UpdateBuffer(gl_uniform_buffer, gl_dynamic_draw);

		defaultVertexBuffer->UpdateBuffer(dimensions);
	}

	virtual void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state)
	{
		ImGui::SetCurrentContext(windowContextMap[window]);
		ImGuiIO& io = ImGui::GetIO();

		switch (button)
		{
			case mouseButton_t::left:
			{
				state == buttonState_t::down ? io.MouseDown[0] = true : io.MouseDown[0] = false;
				break;
			}

			case mouseButton_t::right:
			{
				state == buttonState_t::down ? io.MouseDown[1] = true : io.MouseDown[1] = false;
				break;
			}

			case mouseButton_t::middle:
			{
				state == buttonState_t::down ? io.MouseDown[2] = true : io.MouseDown[2] = false;
				break;
			}
		}
	}

	virtual void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions)
	{
		Resize(window, glm::vec2(dimensions.x, dimensions.y));
	}

	virtual void HandleMaximize(tWindow* window)
	{
		Resize(window);
	}

	virtual void HandleMouseMotion(tWindow* window, vec2_t<int> windowPosition, vec2_t<int> screenPosition)
	{
		ImGui::SetCurrentContext(windowContextMap[window]);
		defaultPayload.data.mousePosition = glm::vec2(windowPosition.x, windowPosition.y);
		UpdateBuffer(gl_uniform_buffer, gl_dynamic_draw);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2((float)windowPosition.x, (float)windowPosition.y); //why screen co-ordinates?
	}

	virtual void HandleMouseWheel(tWindow* window, mouseScroll_t scroll)
	{
		ImGui::SetCurrentContext(windowContextMap[window]);
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += (float)((scroll == mouseScroll_t::down) ? -1 : 1);
	}

	virtual void HandleKey(tWindow* window, int key, keyState_t keyState)
	{
		ImGui::SetCurrentContext(windowContextMap[window]);
		ImGuiIO& io = ImGui::GetIO();
		if (key < 255 && keyState == keyState_t::down)
		{
			io.AddInputCharacter(key);
		}

		else
		{
			switch (keyState)
			{
			case keyState_t::up:
			{
				io.KeysDown[key] = false;
				break;
			}

			case keyState_t::down:
			{
				io.KeysDown[key] = true;
				break;
			}
			}
		}
	}

	virtual void HandleFileDrop(tWindow* window, const std::vector<std::string>& files, const vec2_t<int>& windowMousePosition)
	{
		//for each file that is dropped in
		//make sure its a texture 
		//and load up a new window for each one
	}

	void InitImGUI(tWindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		windowContextMap.insert(std::make_pair(window, ImGui::GetCurrentContext()));

		ImGuiIO& io = ImGui::GetIO();

		io.KeyMap[ImGuiKey_Tab] = TinyWindow::tab;
		io.KeyMap[ImGuiKey_LeftArrow] = TinyWindow::arrowLeft;
		io.KeyMap[ImGuiKey_RightArrow] = TinyWindow::arrowRight;
		io.KeyMap[ImGuiKey_UpArrow] = TinyWindow::arrowUp;
		io.KeyMap[ImGuiKey_PageUp] = TinyWindow::pageUp;
		io.KeyMap[ImGuiKey_PageDown] = TinyWindow::pageDown;
		io.KeyMap[ImGuiKey_Home] = TinyWindow::home;
		io.KeyMap[ImGuiKey_End] = TinyWindow::end;
		io.KeyMap[ImGuiKey_Delete] = TinyWindow::del;
		io.KeyMap[ImGuiKey_Backspace] = TinyWindow::backspace;
		io.KeyMap[ImGuiKey_Enter] = TinyWindow::enter;
		io.KeyMap[ImGuiKey_Escape] = TinyWindow::escape;
		io.KeyMap[ImGuiKey_A] = 'a';
		io.KeyMap[ImGuiKey_C] = 'c';
		io.KeyMap[ImGuiKey_V] = 'v';
		io.KeyMap[ImGuiKey_X] = 'x';
		io.KeyMap[ImGuiKey_Y] = 'y';
		io.KeyMap[ImGuiKey_Z] = 'z';

#if defined(TW_WINDOWS)
		io.ImeWindowHandle = window->GetWindowHandle();
#endif

		imGUIShaderhandle = 0;
		imGUIVertexHandle = 0;
		imGUIFragmentHandle = 0;
		imGUITexAttribLocation = 0;
		imGUIProjMatrixAttribLocation = 0;
		imGUIPositionAttribLocation = 0;
		imGUIUVAttribLocation = 0;
		imGUIColorAttribLocation = 0;
		imGUIVBOHandle = 0;
		imGUIVAOHandle = 0;
		imGUIIBOHandle = 0;
		imGUIFontTexture = 0;

		ImGui::SetCurrentContext(windowContextMap[windows[0]]);
	}

	void HandleImGUIRender(tWindow* window)
	{
		ImDrawData* drawData = ImGui::GetDrawData();

		ImGuiIO& io = ImGui::GetIO();

		drawData->ScaleClipRects(io.DisplayFramebufferScale);

		GLint lastProgram;
		GLint lastTexture;
		GLint lastArrayBuffer;
		GLint lastElementArrayBuffer;
		GLint lastVertexArray;
		GLint lastBlendSrc;
		GLint lastBlendDst;
		GLint lastBlendEquationRGB;
		GLint lastBlendEquationAlpha;
		GLint lastViewport[4];

		glGetIntegerv(gl_current_program, &lastProgram);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGetIntegerv(gl_array_buffer_binding, &lastArrayBuffer);
		glGetIntegerv(gl_element_array_buffer_binding, &lastElementArrayBuffer);
		glGetIntegerv(gl_vertex_array_binding, &lastVertexArray);
		glGetIntegerv(GL_BLEND_SRC, &lastBlendSrc);
		glGetIntegerv(GL_BLEND_DST, &lastBlendDst);
		glGetIntegerv(gl_blend_equation_rgb, &lastBlendEquationRGB);
		glGetIntegerv(gl_blend_equation_alpha, &lastBlendEquationAlpha);
		glGetIntegerv(GL_VIEWPORT, lastViewport);

		GLboolean lastEnableBlend = glIsEnabled(GL_BLEND);
		GLboolean lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
		GLboolean lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
		GLboolean lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

		glEnable(GL_BLEND);
		glBlendEquation(gl_func_add);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glActiveTexture(gl_texture0);

		glViewport(0, 0, window->settings.resolution.width, window->settings.resolution.height);
		const float orthoProjection[4][4] =
		{
			{ 2.0f / (float)window->settings.resolution.width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / -(float)window->settings.resolution.height, 0.0f, 0.0f },
			{ 0.0f, 0.0f, -1.0f, 0.0f },
			{ -1.0f, 1.0f, 0.0f, 1.0f }
		};
		//glm::mat4 testOrtho = glm::perspective(45.0f, )
		glUseProgram(imGUIShaderhandle);
		glUniform1i(imGUITexAttribLocation, 0);
		glUniformMatrix4fv(imGUIProjMatrixAttribLocation, 1, GL_FALSE, &orthoProjection[0][0]);
		glBindVertexArray(imGUIVAOHandle);

		for (int numCommandLists = 0; numCommandLists < drawData->CmdListsCount; numCommandLists++)
		{
			const ImDrawList* commandList = drawData->CmdLists[numCommandLists];
			const ImDrawIdx* indexBufferOffset = 0;

			glBindBuffer(gl_array_buffer, imGUIVBOHandle);
			glBufferData(gl_array_buffer, (GLsizeiptr)commandList->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&commandList->VtxBuffer.front(), gl_stream_draw);

			glBindBuffer(gl_element_array_buffer, imGUIIBOHandle);
			glBufferData(gl_element_array_buffer, (GLsizeiptr)commandList->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&commandList->IdxBuffer.front(), gl_stream_draw);

			for (const ImDrawCmd* drawCommand = commandList->CmdBuffer.begin(); drawCommand != commandList->CmdBuffer.end(); drawCommand++)
			{
				if (drawCommand->UserCallback)
				{
					drawCommand->UserCallback(commandList, drawCommand);
				}

				else
				{
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)drawCommand->TextureId);
					glScissor((int)drawCommand->ClipRect.x, (int)(window->settings.resolution.height - drawCommand->ClipRect.w), (int)(drawCommand->ClipRect.z - drawCommand->ClipRect.x), (int)(drawCommand->ClipRect.w - drawCommand->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)drawCommand->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, indexBufferOffset);
				}
				indexBufferOffset += drawCommand->ElemCount;
			}
		}

		glUseProgram(lastProgram);
		//glActiveTexture(lastActiveTexture);
		glBindTexture(GL_TEXTURE_2D, lastTexture);
		glBindVertexArray(lastVertexArray);
		glBindBuffer(gl_array_buffer, lastArrayBuffer);
		glBindBuffer(gl_element_array_buffer, lastElementArrayBuffer);
		glBlendEquationSeparate(lastBlendEquationRGB, lastBlendEquationAlpha);
		glBlendFunc(lastBlendSrc, lastBlendDst);
		lastEnableBlend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		lastEnableCullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		lastEnableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		lastEnableScissorTest ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
		glViewport(lastViewport[0], lastViewport[1], (GLsizei)lastViewport[2], (GLsizei)lastViewport[3]);
	}

	void ImGUICreateFontsTexture()
	{
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		GLint lastTexture;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGenTextures(1, &imGUIFontTexture);
		glBindTexture(GL_TEXTURE_2D, imGUIFontTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		//store texture handle
		io.Fonts->TexID = (void *)(intptr_t)imGUIFontTexture;

		glBindTexture(GL_TEXTURE_2D, lastTexture);
	}

	void ImGUINewFrame(tWindow* drawWindow)
	{
		if (!imGUIFontTexture)
		{
			ImGUICreateDeviceObjects();
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)drawWindow->settings.resolution.width, (float)drawWindow->settings.resolution.height);
		io.DisplayFramebufferScale = ImVec2(1, 1);
		io.DeltaTime = (float)sceneClock->GetDeltaTime();

		ImGui::NewFrame();
	}

	void ImGUICreateDeviceObjects()
	{
		GLint lastTexture, lastArrayBuffer, LastVertexArray;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGetIntegerv(gl_array_buffer_binding, &lastArrayBuffer);
		glGetIntegerv(gl_vertex_array_binding, &LastVertexArray);

		const char *vertex_shader =
			"#version 330\n"
			"uniform mat4 ProjMtx;\n"
			"in vec2 Position;\n"
			"in vec2 UV;\n"
			"in vec4 Color;\n"
			"out vec2 Frag_UV;\n"
			"out vec4 Frag_Color;\n"
			"void main()\n"
			"{\n"
			"	Frag_UV = UV;\n"
			"	Frag_Color = Color;\n"
			"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
			"}\n";

		const char* fragment_shader =
			"#version 330\n"
			"uniform sampler2D Texture;\n"
			"in vec2 Frag_UV;\n"
			"in vec4 Frag_Color;\n"
			"out vec4 Out_Color;\n"
			"void main()\n"
			"{\n"
			"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
			"}\n";

		imGUIShaderhandle = glCreateProgram();
		imGUIVertexHandle = glCreateShader(gl_vertex_shader);
		imGUIFragmentHandle = glCreateShader(gl_fragment_shader);
		glShaderSource(imGUIVertexHandle, 1, &vertex_shader, nullptr);
		glShaderSource(imGUIFragmentHandle, 1, &fragment_shader, nullptr);
		glCompileShader(imGUIVertexHandle);
		glCompileShader(imGUIFragmentHandle);
		glAttachShader(imGUIShaderhandle, imGUIVertexHandle);
		glAttachShader(imGUIShaderhandle, imGUIFragmentHandle);
		glLinkProgram(imGUIShaderhandle);

		imGUITexAttribLocation = glGetUniformLocation(imGUIShaderhandle, "Texture");
		imGUIProjMatrixAttribLocation = glGetUniformLocation(imGUIShaderhandle, "ProjMtx");
		imGUIPositionAttribLocation = glGetAttribLocation(imGUIShaderhandle, "Position");
		imGUIUVAttribLocation = glGetAttribLocation(imGUIShaderhandle, "UV");
		imGUIColorAttribLocation = glGetAttribLocation(imGUIShaderhandle, "Color");

		glGenBuffers(1, &imGUIVBOHandle);
		glGenBuffers(1, &imGUIIBOHandle);

		glGenVertexArrays(1, &imGUIVAOHandle);
		glBindVertexArray(imGUIVAOHandle);
		glBindBuffer(gl_array_buffer, imGUIVBOHandle);
		glEnableVertexAttribArray(imGUIPositionAttribLocation);
		glEnableVertexAttribArray(imGUIUVAttribLocation);
		glEnableVertexAttribArray(imGUIColorAttribLocation);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(imGUIPositionAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, pos));
		glVertexAttribPointer(imGUIUVAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, uv));
		glVertexAttribPointer(imGUIColorAttribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, col));
#undef OFFSETOF

		ImGUICreateFontsTexture();

		//restore GL state
		glBindTexture(GL_TEXTURE_2D, lastTexture); //why do the values change?
		glBindBuffer(gl_array_buffer, lastArrayBuffer);
		glBindVertexArray(LastVertexArray);
	}

	void ImGUIInvalidateDeviceObject()
	{
		if (imGUIVAOHandle)
		{
			glDeleteVertexArrays(1, &imGUIVAOHandle);
			imGUIVAOHandle = 0;
		}

		if (imGUIVBOHandle)
		{
			glDeleteBuffers(1, &imGUIVBOHandle);
			imGUIVBOHandle = 0;
		}

		if (imGUIIBOHandle)
		{
			glDeleteBuffers(1, &imGUIIBOHandle);
			imGUIIBOHandle = 0;
		}

		glDetachShader(imGUIShaderhandle, imGUIVertexHandle);
		glDeleteShader(imGUIVertexHandle);
		imGUIVertexHandle = 0;

		glDetachShader(imGUIShaderhandle, imGUIFragmentHandle);
		glDeleteShader(imGUIFragmentHandle);
		imGUIFragmentHandle = 0;

		glDeleteProgram(imGUIShaderhandle);
		imGUIShaderhandle = 0;

		if (imGUIFontTexture)
		{
			glDeleteTextures(1, &imGUIFontTexture);
			ImGui::GetIO().Fonts->TexID = nullptr;
			imGUIFontTexture = 0;
		}
	}

	static void APIENTRY OpenGLDebugCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{
		if (severity != gl_debug_severity_low &&
			severity != gl_debug_severity_medium &&
			severity != gl_debug_severity_high)
		{
			//we can skip these for the time being
			return;
		}

		printf("---------------------opengl-callback-start------------\n");
		printf("type: ");
		switch (type)
		{
		case gl_debug_type_error:
		{
			printf("error\n");
			break;
		}

		case gl_debug_type_deprecated_behavior:
		{
			printf("deprecated behavior\n");
			break;
		}

		case gl_debug_type_undefined_behavior:
		{
			printf("undefined behavior\n");
			break;
		}

		case gl_debug_type_performance:
		{
			printf("performance\n");
			break;
		}

		case gl_debug_type_portability:
		{
			printf("portability\n");
			break;
		}
		
		case gl_debug_type_marker:
		{
			printf("marker\n");
			break;
		}

		case gl_debug_type_push_group:
		{
			printf("push group\n");
			break;
		}

		case gl_debug_type_pop_group:
		{
			printf("pop group\n");
			break;
		}
		
		case gl_debug_type_other:
		{
			printf("other\n");
			break;
		}
		}

		printf("ID: %i\n", id);

		printf("severity: ");
		switch (severity)
		{

		case gl_debug_severity_low:
		{
			printf("low \n");
			break;
		}

		case gl_debug_severity_medium:
		{
			printf("medium \n");
			break;
		}

		case gl_debug_severity_high:
		{
			printf("high \n");
			break;
		}
		default:
		{
			printf("\n");
			break;
		}
		}

		printf("Source: ");
		switch (source)
		{
		case gl_debug_source_api:
		{
			printf("API\n");
			break;
		}

		case gl_debug_source_shader_compiler:
		{
			printf("shader compiler\n");
			break;
		}

		case gl_debug_source_window_system:
		{
			printf("window system\n");
			break;
		}

		case gl_debug_source_third_party:
		{
			printf("third party\n");
			break;
		}

		case gl_debug_source_application:
		{
			printf("application\n");
			break;
		}

		case gl_debug_source_other:
		{
			printf("other\n");
			break;
		}
		}

		printf("Message: \n");
		printf("%s \n", message);

		printf("---------------------opengl-callback-end--------------\n");
	}
};
#endif
