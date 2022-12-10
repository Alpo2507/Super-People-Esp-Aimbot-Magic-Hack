#pragma once

#include <windows.h>
#include <string>
#include <TlHelp32.h>

#include "Display.h"

using f_LoadLibraryA = HINSTANCE(WINAPI*)(const char* lpLibFileName);
using f_GetProcAddress = UINT_PTR(WINAPI*)(HINSTANCE hModule, const char* lpProcName);
using f_DLL_ENTRY_POINT = BOOL(WINAPI*)(void* hDll, DWORD dwRead, void* pReserved);

struct MANUAL_MAPPING_DATA
{
	f_LoadLibraryA pLoadLibraryA;
	f_GetProcAddress pGetProcAddress;
	HINSTANCE hMod;
};

namespace Injector
{
	void GetProcessId(const char* processName, DWORD& processId);
	void GetWindowProcessId(const char* windowTitle, DWORD& processId);
	bool DoesFileExist(const std::string& fileName);

	void _stdcall ShellCode(MANUAL_MAPPING_DATA* pData);
	bool ManualMap(HANDLE hProc, std::string& rawData);
	bool Inject(std::string& rawData, const char* processName, bool isProcessWindowTitle);
}

