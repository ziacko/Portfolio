#ifndef SCENE_H
#define SCENE_H
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <stdlib.h>
#include <TinyExtender.h>
#include <TinyShaders.h>
#include <TinyWindow.h>
#include <TinyClock.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <Camera.h>
#include <DefaultUniformBuffer.h> 
//#include <FreeImage.h>
#include "Utilities.h"
#include "VertexBuffer.h"
#include <imgui.h>
#include <stb_image.h>
#include <stb_image_write.h>
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
		defaultUniform = nullptr;
		imGUIFontTexture = 0;

		manager = new windowManager();
		window = manager->AddWindow(windowName, this, 
			TinyWindow::vec2_t<unsigned int>(bufferCamera->resolution.x, bufferCamera->resolution.y),
			4, 5);
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

		glDebugMessageCallback(&OpenGLDebugCallback, NULL);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
			GL_DONT_CARE, 0, NULL, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
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
		manager->resizeEvent = &scene::HandleWindowResize;
		manager->mouseButtonEvent = &scene::HandleMouseClick;
		manager->mouseMoveEvent = &scene::HandleMouseMotion;
		manager->mouseWheelEvent = &scene::HandleMouseWheel;
		manager->maximizedEvent = &scene::HandleMaximize;
		manager->keyEvent = &scene::HandleKey;
		manager->destroyedEvent = &scene::ShutDown;
	}

	static void ShutDown(tWindow* window)
	{
		scene* thisScene = (scene*)window->userData;
		thisScene->imGUIInvalidateDeviceObject();
		tinyShaders::Shutdown();
		thisScene->manager->ShutDown();
	}

	static void RenderImGUIDrawLists(ImDrawData* drawData)
	{
		
	}
	
protected:

	windowManager*				manager;
	tWindow*					window;

	tinyClock_t*				sceneClock;

	defaultUniformBuffer*		defaultUniform;
	vertexBuffer_t*				defaultVertexBuffer;

	camera*						sceneCamera;
	const GLchar*				windowName;
	GLuint						programGLID;
	const GLchar*				tweakBarName;
	const GLchar*				shaderConfigPath;

	GLuint					imGUIFontTexture;
	GLint					imGUIShaderhandle;
	GLint					imGUIVertexHandle;
	GLint					imGUIFragmentHandle;
	GLint					imGUITexAttribLocation;
	GLint					imGUIProjMatrixAttribLocation;
	GLint					imGUIPositionAttribLocation;
	GLint					imGUIUVAttribLocation;
	GLint					imGUIColorAttribLocation;
	GLuint					imGUIVBOHandle;
	GLuint					imGUIVAOHandle;
	GLuint					imGUIIBOHandle;

	bool					isGUIActive;

	virtual void Update()
	{
		manager->PollForEvents();
		sceneCamera->Update();
		sceneClock->UpdateClockAdaptive();
		defaultUniform->deltaTime = (float)sceneClock->GetDeltaTime();
		defaultUniform->totalTime = (float)sceneClock->GetTotalTime();
		
		defaultUniform->framesPerSec = (float)(1.0 / sceneClock->GetDeltaTime());
		UpdateBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
	}

	virtual void Draw()
	{
		glBindVertexArray(defaultVertexBuffer->vertexArrayHandle);
		glUseProgram(this->programGLID);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
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
		
		static int interval = 0;
		if (ImGui::SliderInt("Swap Interval", &interval, 0, 3))
		{
			//manager->SetWindowSwapInterval(window, interval);
		}
	}

	virtual void DrawGUI(const char* guiName, ImVec2 beginSize = ImVec2(0, 0))
	{
		ImGUINewFrame();
		ImGuiIO io = ImGui::GetIO();
		ImGui::Begin(guiName, &isGUIActive, beginSize);
		BuildGUI(io);
		ImGui::End();
		ImGui::Render();
		HandleImGUIRender();
	}

	virtual void SetupVertexBuffer()
	{
		defaultVertexBuffer = new vertexBuffer_t(defaultUniform->resolution);

		GLfloat quadVerts[16] =
		{
			0.0f, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, 0.0f, 1.0f, 1.0f,
			sceneCamera->resolution.x, sceneCamera->resolution.y, 1.0f, 1.0f,
			0.0f, sceneCamera->resolution.y, 1.0f, 1.0f
		};
	}

	static void SetupBuffer(void* buffer, GLuint& bufferHandle, GLuint bufferSize, GLuint bufferUniformHandle, GLenum target, GLenum usage)
	{
		glGenBuffers(1, &bufferHandle);
		UpdateBuffer(buffer, bufferHandle, bufferSize, target, usage);
		glBindBufferBase(target, bufferUniformHandle, bufferHandle);
	}

	//fuh. ill do it AFTER i've fixed GOL
	static void UpdateBuffer(void* buffer, GLuint bufferHandle, GLuint bufferSize, GLenum target, GLenum usage)
	{
		glBindBuffer(target, bufferHandle);
		glBufferData(target, bufferSize, buffer, usage);
	}

	virtual void InitializeBuffers()
	{
		defaultUniform = new defaultUniformBuffer(this->sceneCamera);
		glViewport(0, 0, window->resolution.width, window->resolution.height);
		defaultUniform->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		defaultUniform->projection = glm::ortho(0.0f, (GLfloat)window->resolution.width, (GLfloat)window->resolution.height, 0.0f, 0.01f, 10.0f);

		SetupVertexBuffer();
		SetupBuffer(defaultUniform, defaultUniform->bufferHandle, sizeof(*defaultUniform), 0, GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		//void* blarg = 
	}

	void SetupDefaultUniforms()
	{
		defaultUniform->uniformHandle = glGetUniformBlockIndex(this->programGLID, "defaultSettings");
		glUniformBlockBinding(this->programGLID, defaultUniform->uniformHandle, 0);
	}

	static void HandleMouseClick(tWindow* window, mouseButton_t button, buttonState_t state)
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

	static void HandleWindowResize(tWindow* window, TinyWindow::vec2_t<unsigned int> dimensions)
	{
		scene* thisScene = (scene*)window->userData;

		glViewport(0, 0, dimensions.width, dimensions.height);
		thisScene->defaultUniform->resolution = glm::vec2(dimensions.width, dimensions.height);
		thisScene->defaultUniform->projection = glm::ortho(0.0f, (GLfloat)dimensions.width, (GLfloat)dimensions.height, 0.0f, 0.01f, 10.0f);

		UpdateBuffer(thisScene->defaultUniform, thisScene->defaultUniform->bufferHandle, sizeof(*defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		thisScene->defaultVertexBuffer->UpdateBuffer(thisScene->defaultUniform->resolution);
	}

	static void HandleMouseMotion(tWindow* window, vec2_t<int> windowPosition, vec2_t<int> screenPosition)
	{
		scene* thisScene = (scene*)window->userData;

		thisScene->defaultUniform->mousePosition = glm::vec2(windowPosition.x, windowPosition.y);
		UpdateBuffer(thisScene->defaultUniform, thisScene->defaultUniform->bufferHandle, sizeof(*defaultUniform), GL_UNIFORM_BUFFER, GL_DYNAMIC_DRAW);
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2((float)windowPosition.x, (float)windowPosition.y); //why screen co-ordinates?
	}

	static void HandleMaximize(tWindow* window)
	{
		scene* thisScene = (scene*)window->userData;

		glViewport(0, 0, window->resolution.width, window->resolution.height);
		thisScene->defaultUniform->resolution = glm::vec2(window->resolution.width, window->resolution.height);
		thisScene->defaultUniform->projection = glm::ortho(0.0f, (GLfloat)window->resolution.width, (GLfloat)window->resolution.height, 0.0f, 0.01f, 10.0f);

		//bind the uniform buffer and refill it
		glBindBuffer(GL_UNIFORM_BUFFER, thisScene->defaultUniform->bufferHandle);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(*defaultUniform), thisScene->defaultUniform, GL_DYNAMIC_DRAW);

		thisScene->defaultVertexBuffer->UpdateBuffer(thisScene->defaultUniform->resolution);
	}

	static void HandleMouseWheel(tWindow* window, mouseScroll_t scroll)
	{
		scene* thisScene = (scene*)window->userData;
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += (float)((scroll == mouseScroll_t::down) ? -1 : 1);
	}

	static void HandleKey(tWindow* window, int key, keyState_t keyState)
	{
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
	}

	void HandleImGUIRender()
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
			{ 2.0f / (float)window->resolution.width, 0.0f, 0.0f, 0.0f },
			{ 0.0f, 2.0f / -(float)window->resolution.height, 0.0f, 0.0f },
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

	void imGUIInvalidateDeviceObject()
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

	static void APIENTRY OpenGLDebugCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		printf("---------------------opengl-callback-start------------\n");
		printf("type: ");
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
		{
			printf("error\n");
			break;
		}

		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		{
			printf("deprecated behavior\n");
			break;
		}

		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		{
			printf("undefined behavior\n");
			break;
		}

		case GL_DEBUG_TYPE_PERFORMANCE:
		{
			printf("performance\n");
			break;
		}

		case GL_DEBUG_TYPE_PORTABILITY:
		{
			printf("portability\n");
			break;
		}
		
		case GL_DEBUG_TYPE_MARKER:
		{
			printf("marker\n");
			break;
		}

		case GL_DEBUG_TYPE_PUSH_GROUP:
		{
			printf("push group\n");
			break;
		}

		case GL_DEBUG_TYPE_POP_GROUP:
		{
			printf("pop group\n");
			break;
		}
		
		case GL_DEBUG_TYPE_OTHER:
		{
			printf("other\n");
			break;
		}
		}

		printf("ID: %i\n", id);

		printf("severity: ");
		switch (severity)
		{

		case GL_DEBUG_SEVERITY_LOW:
		{
			printf("low \n");
			break;
		}

		case GL_DEBUG_SEVERITY_MEDIUM:
		{
			printf("medium \n");
			break;
		}

		case GL_DEBUG_SEVERITY_HIGH:
		{
			printf("high \n");
			break;
		}
		}

		printf("Source: ");
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
		{
			printf("API\n");
			break;
		}

		case GL_DEBUG_SOURCE_SHADER_COMPILER:
		{
			printf("shader compiler\n");
			break;
		}

		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		{
			printf("window system\n");
			break;
		}

		case GL_DEBUG_SOURCE_THIRD_PARTY:
		{
			printf("third party\n");
			break;
		}

		case GL_DEBUG_SOURCE_APPLICATION:
		{
			printf("application\n");
			break;
		}

		case GL_DEBUG_SOURCE_OTHER:
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
