#ifndef SCENE_H
#define SCENE_H
#define GLM_SWIZZLE
#include <iostream>
#include <stdlib.h>
#include <TinyExtender.h>
#include <TinyShaders.h>
#include <TinyWindow.h>
#include <TinyClock.h>
#include <fwd.hpp>
#include <glm.hpp>
#include <matrix.hpp>
#include <vec4.hpp>
#include <gtc/matrix_transform.hpp>
#include <Camera.h>
#include <DefaultUniformBuffer.h>
#include <SOIL.h>
#include "Utilities.h"
#include "VertexBuffer.h"
#include <imgui.h>

using namespace TinyWindow;

class scene
{
public:

	scene(const char* windowName = "Ziyad Barakat's Portfolio ( Example Scene )",
		camera* bufferCamera = new camera(),
		const GLchar* shaderConfigPath = "../../resources/shaders/Default.txt")
	{
		this->windowName = windowName;
		this->sceneCamera = bufferCamera;
		this->shaderConfigPath = shaderConfigPath;
		this->tweakBarName = windowName;
		defaultVertexBuffer = nullptr;
		defaultUniformBuffer = nullptr;
		imGUIFontTexture = 0;

		manager = new windowManager();
		window = manager->AddWindow(windowName);
		sceneClock = new tinyClock_t();
	}

	~scene(){}

	virtual void Run()
	{
		while (!window->shouldClose)
		{
			Update();
			Draw();
		}
	}

	virtual void Initialize()
	{
		TinyExtender::InitializeExtensions();
		InitImGUI();
		tinyShaders::LoadShaderProgramsFromConfigFile(this->shaderConfigPath);
		this->programGLID = tinyShaders::GetShaderProgramByIndex(0)->handle;

		glUseProgram(this->programGLID);

		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

		InitializeBuffers();
		SetupCallbacks();
	}

	virtual void SetupCallbacks()
	{
		window->resizeEvent = &scene::HandleWindowResize;
		window->mouseButtonEvent = &scene::HandleMouseClick;
		window->mouseMoveEvent = &scene::HandleMouseMotion;
		window->maximizedEvent = &scene::HandleMaximize;
		window->destroyedEvent = &scene::ShutDown;
	}

	static void ShutDown()
	{
		imGUIInvalidateDeviceObject();
		tinyShaders::Shutdown();
		manager->ShutDown();
	}

	static void RenderImGUIDrawLists(ImDrawData* drawData)
	{
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

		glGetIntegerv(GL_CURRENT_PROGRAM, &lastProgram);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &lastElementArrayBuffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &lastVertexArray);
		glGetIntegerv(GL_BLEND_SRC, &lastBlendSrc);
		glGetIntegerv(GL_BLEND_DST, &lastBlendDst);
		glGetIntegerv(GL_BLEND_EQUATION_RGB, &lastBlendEquationRGB);
		glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &lastBlendEquationAlpha);
		glGetIntegerv(GL_VIEWPORT, lastViewport);

		GLboolean lastEnableBlend = glIsEnabled(GL_BLEND);
		GLboolean lastEnableCullFace = glIsEnabled(GL_CULL_FACE);
		GLboolean lastEnableDepthTest = glIsEnabled(GL_DEPTH_TEST);
		GLboolean lastEnableScissorTest = glIsEnabled(GL_SCISSOR_TEST);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		glActiveTexture(GL_TEXTURE0);

		glViewport(0, 0, window->resolution.width, window->resolution.height);
		const float orthoProjection[4][4] =
		{
			{2.0f / (float)window->resolution.width, 0.0f, 0.0f, 0.0f},
			{0.0f, 2.0f / -(float)window->resolution.height, 0.0f, 0.0f },
			{0.0f, 0.0f, -1.0f, 0.0f},
			{-1.0f, 1.0f, 0.0f, 1.0f }
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

			glBindBuffer(GL_ARRAY_BUFFER, imGUIVBOHandle);
			glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)commandList->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&commandList->VtxBuffer.front(), GL_STREAM_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, imGUIIBOHandle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)commandList->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&commandList->IdxBuffer.front(), GL_STREAM_DRAW);

			for (const ImDrawCmd* drawCommand = commandList->CmdBuffer.begin(); drawCommand != commandList->CmdBuffer.end(); drawCommand++)
			{
				if (drawCommand->UserCallback)
				{
					drawCommand->UserCallback(commandList, drawCommand);
				}

				else
				{
					glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)drawCommand->TextureId);
					glScissor((int)drawCommand->ClipRect.x, (int)(window->resolution.height - drawCommand->ClipRect.w), (int)(drawCommand->ClipRect.z - drawCommand->ClipRect.x), (int)(drawCommand->ClipRect.w - drawCommand->ClipRect.y));
					glDrawElements(GL_TRIANGLES, (GLsizei)drawCommand->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, indexBufferOffset);
				}
				indexBufferOffset += drawCommand->ElemCount;
			}
		}

		glUseProgram(lastProgram);
		//glActiveTexture(lastActiveTexture);
		glBindTexture(GL_TEXTURE_2D, lastTexture);
		glBindVertexArray(lastVertexArray);
		glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lastElementArrayBuffer);
		glBlendEquationSeparate(lastBlendEquationRGB, lastBlendEquationAlpha);
		glBlendFunc(lastBlendSrc, lastBlendDst);
		lastEnableBlend ? glEnable(GL_BLEND) : glDisable(GL_BLEND);
		lastEnableCullFace ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
		lastEnableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
		lastEnableScissorTest ? glEnable(GL_SCISSOR_TEST) : glDisable(GL_SCISSOR_TEST);
		glViewport(lastViewport[0], lastViewport[1], (GLsizei)lastViewport[2], (GLsizei)lastViewport[3]);
	}
	
protected:

	static windowManager*					manager;
	static tWindow*							window;

	tinyClock_t*							sceneClock;

	static defaultUniformBuffer_t*			defaultUniformBuffer;
	static vertexBuffer_t*					defaultVertexBuffer;

	static camera*							sceneCamera;
	const GLchar*							windowName;
	GLuint									programGLID;
	const GLchar*							tweakBarName;
	const GLchar*							shaderConfigPath;

	static GLuint							imGUIFontTexture;

	static GLint							imGUIShaderhandle;
	static GLint							imGUIVertexHandle;
	static GLint							imGUIFragmentHandle;
	static GLint							imGUITexAttribLocation;
	static GLint							imGUIProjMatrixAttribLocation;
	static GLint							imGUIPositionAttribLocation;
	static GLint							imGUIUVAttribLocation;
	static GLint							imGUIColorAttribLocation;
	static GLuint							imGUIVBOHandle;
	static GLuint							imGUIVAOHandle;
	static GLuint							imGUIIBOHandle;

	bool									isGUIActive;

	virtual void Update()
	{
		manager->PollForEvents();
		sceneCamera->Update();
		sceneClock->UpdateClockAdaptive();
		defaultUniformBuffer->deltaTime = sceneClock->GetDeltaTime();
		defaultUniformBuffer->totalTime = sceneClock->GetTotalTime();
		
		defaultUniformBuffer->framesPerSec = 1.0 / sceneClock->GetDeltaTime();
		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
	}

	virtual void Draw()
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);

		glDrawArrays(GL_QUADS, 0, 4);
		
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		DrawGUI(window->name);
		
		window->SwapDrawBuffers();
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual void BuildGUI(ImGuiIO io)
	{
		ImGui::Text("FPS %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, 1.0f / sceneClock->GetDeltaTime());
		ImGui::Text("Total running time %.5f", sceneClock->GetTotalTime());
		ImGui::Text("Mouse coordinates: \t X: %.0f \t Y: %.0f", io.MousePos.x, io.MousePos.y);
		ImGui::Text("Window size: \t Width: %i \t Height: %i", window->resolution.width, window->resolution.height);
	}

	void DrawGUI(const char* guiName, ImVec2 beginSize = ImVec2(0, 0))
	{
		ImGUINewFrame();
		ImGuiIO io = ImGui::GetIO();
		ImGui::Begin(guiName, &isGUIActive, beginSize);
		BuildGUI(io);
		ImGui::End();
		ImGui::Render();
	}

	virtual void SetupVertexBuffer()
	{
		defaultVertexBuffer = new vertexBuffer_t(defaultUniformBuffer->resolution);

		GLfloat quadVerts[16] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, sceneCamera->resolution.y, 1.0f, 1.0f,
			0.0f, sceneCamera->resolution.y, 1.0f, 1.0f
		};
	}

	template<class Type>
	static void SetupUniformBuffer(void* buffer, GLuint& bufferHandle, GLuint bufferUniformHandle)
	{
		glGenBuffers(1, &bufferHandle);
		glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Type), buffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bufferUniformHandle, bufferHandle);
	}

	template<class Type>
	static void UpdateUniformBuffer(void* buffer, GLuint bufferHandle)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(Type), buffer, GL_DYNAMIC_DRAW);
	}

	virtual void InitializeBuffers()
	{
		defaultUniformBuffer = new defaultUniformBuffer_t(this->sceneCamera);
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		//glEnable(GL_DEPTH_TEST);
		defaultUniformBuffer->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)window->resolution.width, (GLfloat)window->resolution.height, 0.0f, 0.01f, 10.0f);

		SetupVertexBuffer();
		SetupUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle, 0);
	}

	void SetupDefaultUniforms()
	{
		defaultUniformBuffer->uniformHandle = glGetUniformBlockIndex(this->programGLID, "defaultSettings");
		glUniformBlockBinding(this->programGLID, defaultUniformBuffer->uniformHandle, 0);
	}

	static void HandleMouseClick(mouseButton_t button, buttonState_t state)
	{
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

	static void HandleWindowResize(TinyWindow::vec2_t<unsigned int> dimensions)
	{
		glViewport(0, 0, dimensions.width, dimensions.height);
		defaultUniformBuffer->resolution = glm::vec2(dimensions.width, dimensions.height);
		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)dimensions.width, (GLfloat)dimensions.height, 0.0f, 0.01f, 10.0f);

		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
		defaultVertexBuffer->UpdateBuffer(defaultUniformBuffer->resolution);

		//ImGuiIO io = ImGui::GetIO();
		//io.DisplaySize = ImVec2(dimensions.width, dimensions.height);

	}

	static void HandleMouseMotion(vec2_t<int> windowPosition, vec2_t<int> screenPosition)
	{
		defaultUniformBuffer->mousePosition = glm::vec2(windowPosition.x, windowPosition.y);
		UpdateUniformBuffer<defaultUniformBuffer_t>(defaultUniformBuffer, defaultUniformBuffer->bufferHandle);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2((float)windowPosition.x, (float)windowPosition.y); //why screen co-ordinates?
	}

	static void HandleMaximize()
	{
		glViewport(0, 0, window->resolution.width, window->resolution.height);

		defaultUniformBuffer->resolution = glm::vec2(window->resolution.width, window->resolution.height);

		defaultUniformBuffer->projection = glm::ortho(0.0f, (GLfloat)window->resolution.width, (GLfloat)window->resolution.height, 0.0f, 0.01f, 10.0f);

		//bind the uniform buffer and refill it
		glBindBuffer(GL_UNIFORM_BUFFER, defaultUniformBuffer->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(defaultUniformBuffer_t), defaultUniformBuffer, GL_DYNAMIC_DRAW);

		defaultVertexBuffer->UpdateBuffer(defaultUniformBuffer->resolution);		
	}

	void InitImGUI()
	{
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

		io.RenderDrawListsFn = RenderImGUIDrawLists;
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

	void ImGUINewFrame()
	{
		if (!imGUIFontTexture)
		{
			ImGUICreateDeviceObjects();
		}

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2((float)window->resolution.width, (float)window->resolution.height);
		io.DisplayFramebufferScale = ImVec2(1, 1);
		io.DeltaTime = (float)sceneClock->GetDeltaTime();

		ImGui::NewFrame();
	}

	void ImGUICreateDeviceObjects()
	{

		GLint lastTexture, lastArrayBuffer, LastVertexArray;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &lastArrayBuffer);
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &LastVertexArray);

		const GLchar *vertex_shader =
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

		const GLchar* fragment_shader =
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
		imGUIVertexHandle = glCreateShader(GL_VERTEX_SHADER);
		imGUIFragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(imGUIVertexHandle, 1, &vertex_shader, 0);
		glShaderSource(imGUIFragmentHandle, 1, &fragment_shader, 0);
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
		glBindBuffer(GL_ARRAY_BUFFER, imGUIVBOHandle);
		glEnableVertexAttribArray(imGUIPositionAttribLocation);
		glEnableVertexAttribArray(imGUIUVAttribLocation);
		glEnableVertexAttribArray(imGUIColorAttribLocation);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
		glVertexAttribPointer(imGUIPositionAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos)); //ffs
		glVertexAttribPointer(imGUIUVAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
		glVertexAttribPointer(imGUIColorAttribLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

		ImGUICreateFontsTexture();

		//restore GL state
		glBindTexture(GL_TEXTURE_2D, lastTexture); //why do the values change?
		glBindBuffer(GL_ARRAY_BUFFER, lastArrayBuffer);
		glBindVertexArray(LastVertexArray);
	}

	static void imGUIInvalidateDeviceObject()
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
			ImGui::GetIO().Fonts->TexID = 0;
			imGUIFontTexture = 0;
		}
	}
};

defaultUniformBuffer_t*		scene::defaultUniformBuffer = nullptr;
vertexBuffer_t*				scene::defaultVertexBuffer = nullptr;
windowManager*				scene::manager = nullptr;
tWindow*					scene::window = nullptr;

GLint						scene::imGUIShaderhandle = 0;
GLint						scene::imGUIVertexHandle = 0;
GLint						scene::imGUIFragmentHandle = 0;
GLint						scene::imGUITexAttribLocation = 0;
GLint						scene::imGUIProjMatrixAttribLocation = 0;
GLint						scene::imGUIPositionAttribLocation = 0;
GLint						scene::imGUIUVAttribLocation = 0;
GLint						scene::imGUIColorAttribLocation = 0;
GLuint						scene::imGUIVBOHandle = 0;
GLuint						scene::imGUIVAOHandle = 0;
GLuint						scene::imGUIIBOHandle = 0;
GLuint						scene::imGUIFontTexture = 0;
camera*						scene::sceneCamera = nullptr;

#endif
