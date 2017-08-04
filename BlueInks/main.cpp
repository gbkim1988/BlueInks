#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include "Debugger.h"
#include "PrimeNumber.h"

#pragma comment( lib, "advapi32.lib" )

using namespace std::literals::chrono_literals;
BOOL WINAPI HandlerRoutine(DWORD dwCtrlType);

HINSTANCE hInst;
HWND hWnd;

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	HANDLE hFile;
	char DataBuffer[] = "This is some test data to write to the file.";
	DWORD dwBytesToWrite = (DWORD)strlen(DataBuffer);
	DWORD dwBytesWritten = 0;
	BOOL bErrorFlag = FALSE;
	hFile = CreateFile(L"C:\\Gura.log",                // name of the write
		GENERIC_WRITE,          // open for writing
		0,                      // do not share
		NULL,                   // default security
		CREATE_NEW,             // create new file only
		FILE_ATTRIBUTE_NORMAL,  // normal file
		NULL);                  // no attr. template

	WriteFile(hFile,
		DataBuffer,
		dwBytesToWrite,
		&dwBytesWritten, // number of bytes that were written
		NULL
	);
	CloseHandle(hFile);

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	DWORD dwThreadIdArray;
	HANDLE hThreadArray;
	switch (message)
	{
	case WM_QUERYENDSESSION:
		return FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		ShutdownBlockReasonCreate(hWnd, L"Don't do it!");
		break;
	case WM_DESTROY:
		hThreadArray = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			0,          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray);   // returns the thread identifier 

		ShutdownBlockReasonDestroy(hWnd);

		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TestClass";
	wcex.hIconSm = NULL;
	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	RECT sz = { 0, 0, 512, 512 };
	AdjustWindowRect(&sz, WS_OVERLAPPEDWINDOW, TRUE);
	hWnd = CreateWindow(L"TestClass", L"Test Window", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, sz.right - sz.left, sz.bottom - sz.top,
		NULL, NULL, hInstance, NULL);
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_C_EVENT ||
		dwCtrlType == CTRL_BREAK_EVENT ||
		dwCtrlType == CTRL_CLOSE_EVENT)
	{
		SendMessage(hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	}
	return FALSE;
}

int main(int argc, char** argv) {
	//struct timeval tv;
	//tv.tv_sec = 1;
	//tv.tv_usec = 500000;
	//select(0, NULL, NULL, NULL, &tv);
	// C++11 에서 동작하는 sleep_for 에 대한 지원 여부를 확인, 가속화 가능? 
	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

	HINSTANCE hInstance = GetModuleHandle(NULL);

	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, SW_HIDE))
	{
		return FALSE;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;

	AntiDebugger::CDebugger* DebuggerDetector = new AntiDebugger::CDebuggerProcessDebugFlag();
	printf("%d \n", PrimeNumbers::PrimeMappers::Maps[0]);
	if (DebuggerDetector->CheckDebuggerPresent() == 1) {
		printf("Debugger Detected");
	}
	else {
		printf("Debugger not Detected");
	}

	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	for (int i = 0; i < 10000; i++) {
		printf("Gura");
		std::this_thread::sleep_for(1000ms);
	}
	system("PAUSE");
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_SHUTDOWN_EVENT:
		//Computer is shutting down

		MessageBox(NULL, L"Gura", L"Gura2", 0);
		
		return FALSE;
	case CTRL_LOGOFF_EVENT:
		//Current user logs off
		MessageBox(NULL, L"Gura", L"Gura2", 0);
		return FALSE;
	default:
		//We don't care about this event
		//Default handler is used
		return FALSE;
	}
	return FALSE;
}