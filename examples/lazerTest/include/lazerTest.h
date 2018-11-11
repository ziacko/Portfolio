#ifndef LAZER_TEST_H
#define LAZER_TEST_H
#define TW_NO_CONSOLE

#include <Scene.h>
#include <strsafe.h>

struct lazerData_t
{
	glm::ivec2					lazerPosition;
	float						objectSize;
	float						objectSpeed;
	bool						objectReceived;
};

static std::vector<const char*> CStrList(std::vector<std::string>& list)
{
	std::vector<const char*> list_;
	for (auto& item : list) list_.push_back(item.c_str());
	return list_;
}

class lazerScene : public scene
{
public:

	lazerScene(
		const char* windowName = "Impact wall laser grid editor",
		camera* contrastCamera = new camera())
		: scene(windowName, contrastCamera, shaderConfigPath)
	{
		serialConnected = false;
		popupName = "about Lazer grid settings";
		baudRate = 57600;
		emulateMouseClick = true;
		fakeMousedown = false;
#if defined(TW_WINDOWS)
		mouseBuffer = new INPUT();
#endif

	}

	~lazerScene(){};

	void Initialize() override
	{
		//scene::Initialize();
		TinyExtender::InitializeExtensions();

		glDebugMessageCallback(&OpenGLDebugCallback, NULL);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
			GL_DONT_CARE, 0, NULL, GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		InitImGUI();
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

		InitializeUniforms();
		SetupCallbacks();

		SetGuiStyle();
		SerialPortInit();
		for (auto monitor : manager->GetMonitors())
		{
			if (monitor->isPrimary)
			{
				currentMonitor = monitor;
			}
		}
		
	}

protected:

	//imgui variables
	bool							displayAboutwindow;
	std::string						popupName;
	int								activeBaudRate = 0;
	std::vector<std::string>		baudRates =
	{
		"600",
		"1200",
		"4800",
		"9600",
		"14400",
		"19200",
		"28800",
		"38400",
		"56000",
		"57600",
		"74880",
		"115200",
		"128000",
		"153600",
		"230400",
		"250000",
		"256000",
		"460800",
		"921600",
	};
	bool							fakeMousedown;
	bool							emulateMouseClick;

	//serial variables
	bool							serialConnected;
	char							serialPort[255] = "";
	int								baudRate;
	monitor_t*						currentMonitor;
	std::vector<lazerData_t>		inputs;
	std::string						inputBuffer;
#if defined(TW_WINDOWS)
	HANDLE							serialHandle;
	COMSTAT							serialStatus;
	DWORD							serialErrors;
	INPUT*							mouseBuffer;
#endif

	int SerialPortInit()
	{
		
		serialHandle = CreateFile(serialPort, GENERIC_READ,
			0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		//was the connection successful?
		if (serialHandle == INVALID_HANDLE_VALUE)
		{
			if (GetLastError() == ERROR_FILE_NOT_FOUND)
			{
				printf("Error: unable to open port: %s\n", serialPort);
				return -1;
			}

			else
			{
				printf("ERROR \n");
				return -1;
			}
		}

		else
		{
			//if connected set the comm parameters
			DCB serialParams = { 0 };

			if (!GetCommState(serialHandle, &serialParams))
			{
				//show error if you can't get the current parameters
				printf("failed to get current serial parameters! \n");
				return -1;
			}

			else
			{
				//define serial connection parameters for the arduino UNO
				serialParams.BaudRate = baudRate;
				serialParams.ByteSize = 8;
				serialParams.StopBits = ONESTOPBIT;
				serialParams.Parity = NOPARITY;

				//setting the DTR to enable ensures that the arduino
				//is properly reset upon establishing a connection
				serialParams.fDtrControl = DTR_CONTROL_ENABLE;
				if (!SetCommState(serialHandle, &serialParams))
				{
					printf("ALERT: could not set Serial port parameters \n");
				}

				else
				{
					//if everything is fine we're connected
					serialConnected = true;
					//flush any remaining characters on the buffers
					PurgeComm(serialHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
				}
			}
		}

		return 0;
	}

	void ClosePort()
	{
		CloseHandle(serialHandle);
		serialHandle = NULL;
	}

	void NewPort()
	{
		ClosePort();
		SerialPortInit();
	}
/*
	int SerialPortClose(int fd)
	{
		return 0;
	}

	int SerialPortWriteByte(int fd, unsigned int bit)
	{
		return 0;
	}

	int SerialPortWrite(int fd, std::string message)
	{
		return 0;
	}

	int SerialPortReadUntil(int fd, std::string buffer, char untilCharRead, int maxBufferSize, int timeOut)
	{
		return 0;
	}

	lazerData_t SerialPortRead(int fd)
	{
		return lazerData_t();
	}

	int SerialPortFlush(int fd)
	{
		return 0;
	}

	int InitSerial()
	{
		return 0;
	}

	void Error(LPTSTR lpszFunction)
	{
		// Retrieve the system error message for the last-error code

		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			lpszFunction, dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}*/

	void ReadData(unsigned int fd)
	{
		DWORD bytesRead = 0;
		unsigned int toRead = 0;
		unsigned char buffer[255] = { 0 };		
		lazerData_t newData = {};
		
		//get status info on the serial port
		ClearCommError(serialHandle, &serialErrors, &serialStatus);
		//if there is something to read
		if (serialStatus.cbInQue > 0)
		{
			//check if there is enough data to read the required number 
			//of characters. If not we'll read only the available characters to prevent
			//locking of the application
			if (serialStatus.cbInQue > fd)
			{
				toRead = fd;
			}
			else
			{
				toRead = serialStatus.cbInQue;
			}
			
			if (ReadFile(serialHandle, buffer, toRead, &bytesRead, NULL))
			{

				newData.lazerPosition.x = buffer[0];
				newData.lazerPosition.y = buffer[1];

				if (emulateMouseClick)
				{
					int widthUpscaled = currentMonitor->extents.right / 255;
					int heightUpscaled = currentMonitor->extents.bottom / 255;
					int xUpscaled = newData.lazerPosition.x * widthUpscaled + currentMonitor->extents.left;
					int yUpscaled = newData.lazerPosition.y * heightUpscaled + currentMonitor->extents.top;

					//upscale resolutions to current monitor and add to mouse clicks
					FakeMouseClickDown(glm::ivec2(xUpscaled, yUpscaled));
				}

				inputs.push_back(newData);

				if (inputs.size() > 100)
				{
					//cut the fat from the back of it.
					inputs.erase(inputs.begin());
				}
			}
		}
	}

	void DrawGUI(const char* guiName, ImVec2 beginSize = ImVec2(0, 0)) override
	{
		ImGUINewFrame();
		ImGuiIO io = ImGui::GetIO();
		
		ImGui::SetNextWindowSize(ImVec2(window->resolution.x, window->resolution.y));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::Begin(guiName, nullptr, windowFlags);
		BuildGUI(io);
		ImGui::End();
		ImGui::Render();
		HandleImGUIRender();
	}

	void BuildGUI(ImGuiIO io) override
	{
		DrawMenu();
		DrawColumn1();
		DrawColumn2();
		//DrawColumn3();
	}

	void DrawMenu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("About", NULL, nullptr))
				{
					displayAboutwindow = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void DrawMeta()
	{
		ImGui::TextColored(ImVec4(0.067f, 0.765f, 0.941f, 1.0f), "Impact Wall Laser grid Editor");
		ImGui::TextColored(ImVec4(0.75f, 0.75f, 0.75f, 1.0f), "(c) Copyright 2016 Atmos Studios");
		ImGui::Spacing(); ImGui::Spacing();
	}

	void DrawColumn1()
	{
		ImGui::BeginChild("Column1", ImVec2(350, 0), true);
		DrawMeta();
		ImGui::Separator();
		ImGui::Checkbox("Emulate mouse clicks", &emulateMouseClick);
		//enter the port and baud rate
		ImGui::Text("Port Name");		
		ImGui::InputText("##Port Name", serialPort, 255);
		ImGui::Text("Baud rate: %i", baudRate);
		//pick from a list of baud rates
		if (ImGui::ListBox((std::string("##") + "Baud Rate").c_str(), &activeBaudRate, CStrList(baudRates).data(), baudRates.size()))
		{
			baudRate = std::stoi(baudRates[activeBaudRate]);
		}
		//add a button to reset connection
		if (ImGui::Button("reset serial connection"))
		{
			NewPort();
		}
		//cut the connection as well
		if (ImGui::Button("Close connection"))
		{
			ClosePort();
		}
		ImGui::Separator();
		//pick which screen to use. be sure to display the screen name and resolution
		ImGui::TextColored(ImVec4(0.067f, 0.765f, 0.941f, 1.0f), "attached monitors");
		//for each monitor attached, display the resolution and name
		for (auto monitor : manager->GetMonitors())
		{
			ImGui::Text("Device Name: %s", monitor->deviceName.c_str());
			ImGui::Text("Display Name: %s", monitor->displayName.c_str());
			ImGui::Text("Monitor Name: %s", monitor->monitorName.c_str());
			ImGui::Text("Resolution: Width: %i Height: %i", monitor->currentSetting->resolution.width, monitor->currentSetting->resolution.height);
			ImGui::Text("Is Primary Monitor: %s", (monitor->isPrimary ? "true" : "false"));
			ImGui::Text("Extents: Top: %i Bottom: %i Left: %i Right: %i", monitor->extents.top, monitor->extents.bottom, monitor->extents.left, monitor->extents.right);
			if (ImGui::Button("Use this screen?"))
			{
				currentMonitor = monitor;
				//if pressed use this screen as our reference
			}
		}

		ImGui::EndChild();
		ImGui::SameLine();
	}

	void DrawColumn2()
	{
		ImGui::BeginChild("Column2", ImVec2(350, 0), true, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::TextColored(ImVec4(0.067f, 0.765f, 0.941f, 1.0f), "laser input coordinates | upscaled");
		ImVec2 windowDimensions = ImGui::GetWindowSize();
		ImVec2 newDimensions = ImVec2(windowDimensions.x, windowDimensions.y - 4);
		//build up the string here. maybe move it later?
		for (auto laser : inputs)
		{
			//add X
			//add Y
			//add formatted X
			int XUpscaled = currentMonitor->extents.right / 255;
			int YUpscaled = currentMonitor->extents.bottom/ 255;
			//add formatted Y
			inputBuffer += "X: " + std::to_string(laser.lazerPosition.x) + " ";
			inputBuffer += "Y: " + std::to_string(laser.lazerPosition.y) + " ";
			inputBuffer += "\t\t|\t ";
			inputBuffer += "X: " + std::to_string((laser.lazerPosition.x * XUpscaled) + currentMonitor->extents.left) + " ";
			inputBuffer += "X: " + std::to_string((laser.lazerPosition.y * YUpscaled) + currentMonitor->extents.top) + "\n";
		}
		ImGui::InputTextMultiline("##", (char*)inputBuffer.c_str(), inputBuffer.size() * sizeof(char), newDimensions, ImGuiInputTextFlags_ReadOnly);
		inputBuffer.clear();
		ImGui::EndChild();
		ImGui::SameLine();

	}

	void DrawColumn3()
	{
		ImGui::BeginChild("Column3", ImVec2(350, 0), true);
		ImGui::TextColored(ImVec4(0.067f, 0.765f, 0.941f, 1.0f), "laser inputs coordinates (upscaled to screen resolution)");
		ImGui::EndChild();
	}

	void SetupFakeMouse()
	{
		mouseBuffer->type = INPUT_MOUSE;
		mouseBuffer->mi.dx = (0 * (0xFFFF / currentMonitor->currentSetting->resolution.width));
		mouseBuffer->mi.dy = (0 * (0xFFFF / currentMonitor->currentSetting->resolution.height));
		mouseBuffer->mi.mouseData = 0;
		mouseBuffer->mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP;
		mouseBuffer->mi.time = 0;
		mouseBuffer->mi.dwExtraInfo = 0;
	}

	void FakeMouseClickDown(glm::ivec2 mousePosition)
	{
		fakeMousedown = true;
		mouseBuffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN);
		mouseBuffer->mi.dx = (mousePosition.x * (0xFFFF / currentMonitor->extents.right));
		mouseBuffer->mi.dy = (mousePosition.y * (0xFFFF / currentMonitor->extents.bottom));
		SendInput(1, mouseBuffer, sizeof(INPUT));
	}

	void MouseClickUp()
	{
		mouseBuffer->mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP);
		SendInput(1, mouseBuffer, sizeof(INPUT));
	}

	void Update() override
	{
		//wait for events is not an option here. we must poll to receive continuous input
		scene::Update();
		//UpdateBuffer(contrastSettings, contrastSettings->bufferHandle, sizeof(*contrastSettings), gl_uniform_buffer, gl_dynamic_draw);

		ReadData(255);

		if (fakeMousedown)
		{
			MouseClickUp();
			fakeMousedown = false;
		}
	}

	void SetGuiStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		style.GrabRounding = style.ScrollbarRounding = style.FrameRounding = 2;
		style.WindowRounding = 0;
		style.ItemSpacing.x = 4; style.FramePadding.x = 6;
		style.Colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.84f, 0.84f, 0.84f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.90f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.15f, 0.15f, 0.15f, 0.09f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.78f, 0.80f, 0.80f, 0.30f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.37f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.22f, 0.60f, 0.82f, 0.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.88f, 0.88f, 0.88f, 0.45f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.82f, 0.82f, 0.82f, 0.90f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 0.91f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.53f, 0.53f, 0.53f, 0.67f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.53f, 0.53f, 0.53f, 0.82f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 0.00f, 0.00f, 0.15f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.82f, 0.82f, 0.82f, 0.67f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_ComboBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.99f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.50f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.82f, 0.82f, 0.82f, 0.67f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.81f, 0.82f, 0.82f, 0.77f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.22f, 0.60f, 0.82f, 0.50f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.60f, 0.82f, 0.70f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_Column] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_ColumnHovered] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.30f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		style.Colors[ImGuiCol_CloseButton] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.65f, 0.22f, 0.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.93f, 0.52f, 0.02f, 0.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.92f, 0.82f, 0.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.22f, 0.60f, 0.82f, 1.00f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.22f);

		ImFontConfig cfg;
		memset(&cfg, 0, sizeof(ImFontConfig));
		cfg.OversampleH = 2;
		cfg.OversampleV = 2;
		cfg.PixelSnapH = true;
		cfg.GlyphExtraSpacing.x = 0.1f;
		cfg.GlyphExtraSpacing.y = 0.0f;
	}
};

#endif //LAZER_TEST_H
