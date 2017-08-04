#ifndef DEBUGGER_H_
#define DEBUGGER_H_
#include "Debugger.h"


AntiDebugger::CDebugger::CDebugger()
{
	dwDebuggerPresent = 0xffffffff;
}


AntiDebugger::CDebugger::~CDebugger()
{
}

DWORD AntiDebugger::CDebuggerProcessDebugFlag::CheckDebuggerPresent() {
	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)(HANDLE, UINT, PVOID, ULONG, PULONG);

	DWORD NoDebugInherit = 0;
	NTSTATUS Status;

	HMODULE hNTDLL = GetModuleHandle(L"ntdll.dll");
	if (hNTDLL == INVALID_HANDLE_VALUE)
	{
		//sErrorMessage = TEXT("Failed to load ntdll");
		return 0xfffffffe;
	}

	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)GetProcAddress(hNTDLL, "NtQueryInformationProcess");
	if (NtQIP == NULL)
	{
		//sErrorMessage = TEXT("Failed to load NtQueryInformationProcess");
		return 0xfffffffe;
	}

	Status = NtQIP(GetCurrentProcess(), 0x1f, &NoDebugInherit, 4, NULL);
	if (Status != 0x00000000)
	{
		//sErrorMessage = TEXT("Error in NtQueryInformationProcess");
		return 0xfffffffe;
	}

	if (NoDebugInherit == FALSE)
		return 0xB9DD;
	else
		return 0xffffffff;
}

#endif