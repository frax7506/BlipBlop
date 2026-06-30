#include "stdafx.h"

#include "Application.h"
#include "ModelViewer.h"

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

int GuardedMain()
{
#if !IS_RETAIL_BUILD
	// Request a console window.
	AllocConsole();

	// Redirect stdout and stderr to the console. (std::cout and std::cerr)
	FILE* consoleOut;
	FILE* consoleErr;
	freopen_s(&consoleOut, "CONOUT$", "w", stdout);  // NOLINT(cert-err33-c)
	setvbuf(consoleOut, nullptr, _IONBF, 1024);  // NOLINT(cert-err33-c)
	freopen_s(&consoleErr, "CONOUT$", "w", stderr);  // NOLINT(cert-err33-c)
	setvbuf(consoleErr, nullptr, _IONBF, 1024);  // NOLINT(cert-err33-c)

	// Resize the console window to 1280 x 720 and force update.
	const HWND consoleWindow = GetConsoleWindow();
	RECT consoleSize;
	GetWindowRect(consoleWindow, &consoleSize);
	MoveWindow(consoleWindow, consoleSize.left, consoleSize.top, 1280, 720, true);

	SetConsoleOutputCP(CP_UTF8);
#endif // !IS_RETAIL_BUILD

	LOG_MESSAGE("ModelViewer starting...");

	constexpr SIZE windowSize = { 1920, 1080 };
	constexpr LPCWSTR windowTitle = L"AGP Modelviewer";

	ModelViewer modelViewer;
	modelViewer.Init(windowSize, WinProc, windowTitle);
	return modelViewer.Run();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	HD_Logger::GetInstance().Init();

	int returnValue = 0;

	try
	{
		returnValue = GuardedMain();
	}
	catch (const std::exception& aException)
	{
		HD_String message = aException.what();
		LOG_ERROR_F("Exception caught: %s\n", message.GetBuffer());
		returnValue = -1;
	}

	HD_Logger::GetInstance().DeInit();
	return returnValue;
}

LRESULT CALLBACK WinProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	HD_InputManager::GetInstance().UpdateEvents(uMsg, wParam, lParam);

	if (uMsg == WM_DESTROY || uMsg == WM_CLOSE)
	{
		PostQuitMessage(0);
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
