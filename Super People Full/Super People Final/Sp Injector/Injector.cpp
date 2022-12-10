#include "Injector.h"

void Injector::GetProcessId(const char* processName, DWORD& processId)
{
	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return;
	}

	BOOL bRet = Process32First(hSnap, &PE32);
	while (bRet)
	{
		if (!strcmp(processName, PE32.szExeFile))
		{
			processId = PE32.th32ProcessID;
		}

		bRet = Process32Next(hSnap, &PE32);
	}

	CloseHandle(hSnap);
}

void Injector::GetWindowProcessId(const char* windowTitle, DWORD& processId)
{
	GetWindowThreadProcessId(FindWindow(nullptr, windowTitle), &processId);
}

bool Injector::DoesFileExist(const std::string& fileName)
{
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}

// Injector Funcs
bool Injector::ManualMap(HANDLE hProc, std::string& rawData)
{
	BYTE* pSrcData = nullptr;
	BYTE* pTargetBase = nullptr;
	IMAGE_NT_HEADERS* pOldNtHeader = nullptr;
	IMAGE_OPTIONAL_HEADER* pOldOptHeader = nullptr;
	IMAGE_FILE_HEADER* pOldFileHeader = nullptr;

	pSrcData = reinterpret_cast<BYTE*>(&rawData[0]);
	
	if (reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_magic != IMAGE_DOS_SIGNATURE) //"MZ"
	{
		DisplayError(ErrCode::InvalidRawFile);
		return false;
	}

	pOldNtHeader = reinterpret_cast<IMAGE_NT_HEADERS*>(pSrcData + reinterpret_cast<IMAGE_DOS_HEADER*>(pSrcData)->e_lfanew);
	
	if (pOldNtHeader->Signature != IMAGE_NT_SIGNATURE)
	{
		DisplayError(ErrCode::InvalidSignature);
		return false;
	}

	pOldOptHeader = &pOldNtHeader->OptionalHeader;
	pOldFileHeader = &pOldNtHeader->FileHeader;

#ifdef _WIN64
	if (pOldFileHeader->Machine != IMAGE_FILE_MACHINE_AMD64)
	{
		DisplayError(ErrCode::InvalidPlatform);
		return false;
	}
#else
	if (pOldFileHeader->Machine != IMAGE_FILE_MACHINE_I386)
	{
		DisplayError(ErrCode::InvalidPlatform);
		return false;
	}
#endif

	pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, reinterpret_cast<void*>(pOldOptHeader->ImageBase), pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
	if (!pTargetBase)
	{
		pTargetBase = reinterpret_cast<BYTE*>(VirtualAllocEx(hProc, nullptr, pOldOptHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
		if (!pTargetBase)
		{
			DisplayError(ErrCode::MemTargetAllocFailed);
			return false;
		}
	}

	MANUAL_MAPPING_DATA		data{ nullptr };
	data.pLoadLibraryA = LoadLibraryA;
	data.pGetProcAddress = reinterpret_cast<f_GetProcAddress>(GetProcAddress);

	auto* pSectioHeader = IMAGE_FIRST_SECTION(pOldNtHeader);
	for (UINT i{ 0 }; i != pOldFileHeader->NumberOfSections; i++, pSectioHeader++)
	{
		if (pSectioHeader->SizeOfRawData)
		{
			if (!WriteProcessMemory(hProc, pTargetBase + pSectioHeader->VirtualAddress, pSrcData + pSectioHeader->PointerToRawData, pSectioHeader->SizeOfRawData, nullptr))
			{
				DisplayError(ErrCode::FailedMapSections);
				// Deallocate Memory in the target process
				VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
				return false;
			}
		}
	}

	memcpy(pSrcData, &data, sizeof(data));
	// 0x1000 reserve for the headers
	WriteProcessMemory(hProc, pTargetBase, pSrcData, 0x1000, nullptr);

	void* pShellcode = VirtualAllocEx(hProc, nullptr, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (!pShellcode)
	{
		DisplayError(ErrCode::MemAllocShellFailed);
		VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
		return false;
	}

	WriteProcessMemory(hProc, pShellcode, ShellCode, 0x1000, nullptr);

	HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(pShellcode), pTargetBase, 0, nullptr);
	if (!hThread)
	{
		DisplayError(ErrCode::ThreadCreationFailed);
		VirtualFreeEx(hProc, pTargetBase, 0, MEM_RELEASE);
		VirtualFreeEx(hProc, pShellcode, 0, MEM_RELEASE);
		return false;
	}

	CloseHandle(hThread);

	HINSTANCE hCheck = nullptr;
	while (!hCheck)
	{
		MANUAL_MAPPING_DATA data_checked{ nullptr };
		ReadProcessMemory(hProc, pTargetBase, &data_checked, sizeof(data_checked), nullptr);
		hCheck = data_checked.hMod;
		Sleep(10);
	}

	VirtualFreeEx(hProc, pShellcode, 0, MEM_RELEASE);

	return true;
}

#define RELOC_FLAG32(RelInfo) (((RelInfo) >> 0x0c) == IMAGE_REL_BASED_HIGHLOW)
#define RELOC_FLAG64(RelInfo) (((RelInfo) >> 0x0c) == IMAGE_REL_BASED_DIR64)

#ifdef _WIN64
#define RELOC_FLAG RELOC_FLAG64
#else
#define RELOC_FLAG RELOC_FLAG32
#endif

void _stdcall Injector::ShellCode(MANUAL_MAPPING_DATA* pData)
{
	if (!pData) return;

	BYTE* pBase = reinterpret_cast<BYTE*>(pData);
	auto* pOpt = &reinterpret_cast<IMAGE_NT_HEADERS*>(pBase + reinterpret_cast<IMAGE_DOS_HEADER*>(pData)->e_lfanew)->OptionalHeader;

	auto _LoadLibraryA = pData->pLoadLibraryA;
	auto _GetProcAddress = pData->pGetProcAddress;
	auto _DllMain = reinterpret_cast<f_DLL_ENTRY_POINT>(pBase + pOpt->AddressOfEntryPoint);

	BYTE* LocationDelta = pBase - pOpt->ImageBase;
	if (LocationDelta)
	{
		if (!pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) return;

		auto* pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pRelocData->VirtualAddress)
		{
			UINT AmountOfEntries = (pRelocData->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
			WORD* pRelativeInfo = reinterpret_cast<WORD*>(pRelocData + 1);

			// We care about one type of rellocation
			for (UINT i{ 0 }; i != AmountOfEntries; i++, pRelativeInfo++)
			{
				if (RELOC_FLAG(*pRelativeInfo))
				{
					UINT_PTR* pPatch = reinterpret_cast<UINT_PTR*>(pBase + pRelocData->VirtualAddress + ((*pRelativeInfo) & 0xFFF));
					*pPatch += reinterpret_cast<UINT_PTR>(LocationDelta);
				}
			}

			pRelocData = reinterpret_cast<IMAGE_BASE_RELOCATION*>(reinterpret_cast<BYTE*>(pRelocData) + pRelocData->SizeOfBlock);
		}
	}

	if (pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size)
	{
		auto* pImportDescriptor = reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		while (pImportDescriptor->Name)
		{
			char* hModule = reinterpret_cast<char*>(pBase + pImportDescriptor->Name);
			HINSTANCE hDll = _LoadLibraryA(hModule);

			ULONG_PTR* pThunkRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDescriptor->OriginalFirstThunk);
			ULONG_PTR* pFuncRef = reinterpret_cast<ULONG_PTR*>(pBase + pImportDescriptor->FirstThunk);

			if (!pThunkRef) pThunkRef = pFuncRef;

			while (*pThunkRef)
			{

				if (IMAGE_SNAP_BY_ORDINAL(*pThunkRef))
				{
					*pFuncRef = _GetProcAddress(hDll, reinterpret_cast<char*>(*pThunkRef & 0xFFFF));
				}
				else
				{
					auto* pImport = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(pBase + (*pThunkRef));
					*pFuncRef = _GetProcAddress(hDll, pImport->Name);
				}

				pThunkRef++;
				pFuncRef++;
			}

			pImportDescriptor++;
		}
	}

	if (pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size)
	{
		auto* pTLS = reinterpret_cast<IMAGE_TLS_DIRECTORY*>(pBase + pOpt->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		auto* pCallBack = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(pTLS->AddressOfCallBacks);

		while (pCallBack && *pCallBack)
		{
			(*pCallBack)(pBase, DLL_PROCESS_ATTACH, nullptr);
			pCallBack++;
		}
	}

	_DllMain(pBase, DLL_PROCESS_ATTACH, nullptr);

	pData->hMod = reinterpret_cast<HINSTANCE>(pBase);
}

bool Injector::Inject(std::string& rawData, const char* processName, bool isProcessWindowTitle)
{
	DWORD PID = 0;
	
	if (isProcessWindowTitle)
	{
		GetWindowProcessId(processName, PID);
	}
	else
	{
		GetProcessId(processName, PID);
	}

	if (!PID)
	{
		DisplayError(ErrCode::FailedToGetProcId);
		return false;
	}

	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, false, PID);
	if (!hProc)
	{
		DisplayError(ErrCode::FailedToOpenProc);
		return false;
	}

	if (!ManualMap(hProc, rawData))
	{
		CloseHandle(hProc);
		DisplayInfo(InfoCode::InjectionAborted);
		return false;
	}

	CloseHandle(hProc);
	return true;
}