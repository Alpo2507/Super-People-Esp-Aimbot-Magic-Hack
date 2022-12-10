#include "pch.h"

#define MM_DEVICE_TYPE 0x9999
#define MM_CTL_CODE(x) CTL_CODE(MM_DEVICE_TYPE, 0x800 + x, METHOD_NEITHER, FILE_ANY_ACCESS)
#define MM_READVIRTUALMEMORY MM_CTL_CODE(56)

typedef struct _MEMORY_REGION
{
	DWORD_PTR dwBaseAddr;
	DWORD_PTR dwMemorySize;
}MEMORY_REGION;

MemoryManager* g_pMemoryManager = new MemoryManager();
MemoryManager::MemoryManager() {}
MemoryManager::~MemoryManager() {}

bool MemoryManager::init()
{
	string deviceName = "\\Device\\KProcessHacker2";
	wstring strObjectName;
	strObjectName.assign(deviceName.begin(), deviceName.end());
	UNICODE_STRING objectName;
	HANDLE hNtDriver;
	OBJECT_ATTRIBUTES objectAttributes;
	IO_STATUS_BLOCK isb;
	RtlInitUnicodeString(&objectName, strObjectName.c_str());
	InitializeObjectAttributes(&objectAttributes, &objectName, FILE_NON_DIRECTORY_FILE, NULL, NULL);
	NTSTATUS status = NtOpenFile(&hNtDriver, FILE_GENERIC_READ, &objectAttributes, &isb, FILE_SHARE_READ, FILE_NON_DIRECTORY_FILE);
	if (status == STATUS_SUCCESS) {
		m_hDriver = hNtDriver;
		return true;
	}
	return false;
}

int MemoryManager::getAowProcId()
{
	int pid = 0;
	int threadCount = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnap, &pe);
	while (Process32Next(hSnap, &pe)) {
		if (_tcsicmp(pe.szExeFile, _T("aow_exe.exe")) == 0) {
			if ((int)pe.cntThreads > threadCount) {
				threadCount = pe.cntThreads;
				pid = pe.th32ProcessID;
			}
		}
	}
	return pid;
}

bool MemoryManager::getProcHandle()
{
	// Get process handle
	processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (processHandle)
	{
		allIsWell = true;
		return true;
	}
	else
	{
		return false;
	}
}

void MemoryManager::readMemory(PVOID BaseAddress, PVOID Buffer, SIZE_T BufferSize)
{
	if (!allIsWell)
		return;

	struct {
		HANDLE ProcessHandle;
		PVOID BaseAddress;
		PVOID Buffer;
		SIZE_T BufferSize;
		PSIZE_T NumberOfBytesRead;
	} input = { processHandle, BaseAddress, Buffer, BufferSize, NumberOfBytes };

	IO_STATUS_BLOCK ioStatusBlock;

	NtDeviceIoControlFile(m_hDriver, nullptr, nullptr, nullptr, &ioStatusBlock, MM_READVIRTUALMEMORY, &input, sizeof(input), nullptr, 0);
}


bool MemoryManager::search(BYTE* bSearchData, int nSearchSize, DWORD_PTR dwStartAddr, DWORD_PTR dwEndAddr, vector<DWORD_PTR>& vRet)
{

	MEMORY_BASIC_INFORMATION mbi;
	vector<MEMORY_REGION> m_vMemoryRegion;
	mbi.RegionSize = 0x400;
	DWORD dwAddress = dwStartAddr;

	MEMORY_REGION memSectorList[1000];

	int memSectorIndex = 0;
	while ((VirtualQueryEx(processHandle, (LPCVOID)dwAddress, &mbi, sizeof(mbi))) && (dwAddress < dwEndAddr) && ((dwAddress + mbi.RegionSize) > dwAddress)) {
		if (
			(mbi.State == MEM_COMMIT) &&
			((mbi.Protect & PAGE_GUARD) == 0) &&
			(mbi.Protect != PAGE_NOACCESS) &&
			((mbi.AllocationProtect & PAGE_NOCACHE) != PAGE_NOCACHE)
			) {
			MEMORY_REGION mData = { 0 };
			mData.dwBaseAddr = (DWORD_PTR)mbi.BaseAddress;
			mData.dwMemorySize = mbi.RegionSize;
			m_vMemoryRegion.push_back(mData);
			memSectorList[memSectorIndex] = mData;
			memSectorIndex++;
		}
		dwAddress = (DWORD)mbi.BaseAddress + mbi.RegionSize;
	}
	vector<MEMORY_REGION>::iterator it;
	int memSectorCount = memSectorIndex;
	memSectorIndex = 0;
	DWORD_PTR curAddr = dwStartAddr;
	while (curAddr < dwEndAddr)
	{
		VirtualQueryEx(processHandle, (LPCVOID)curAddr, &mbi, sizeof(mbi));
		long regionSizeOrg = mbi.RegionSize;
		long regionSize = mbi.RegionSize;
		if (regionSize > 10) {
			BYTE* pCurrMemoryData = new BYTE[regionSize];
			ZeroMemory(pCurrMemoryData, regionSize);
			readMemory((PVOID)curAddr, (PVOID*)pCurrMemoryData, regionSize);
			DWORD_PTR dwOffset = 0;
			int iOffset = find(pCurrMemoryData, regionSize, bSearchData, nSearchSize);
			while (iOffset != -1) {
				dwOffset += iOffset;
				vRet.push_back(dwOffset + curAddr);
				dwOffset += nSearchSize;
				iOffset = find(pCurrMemoryData + dwOffset, regionSize - dwOffset - nSearchSize, bSearchData, nSearchSize);
			}
			delete[] pCurrMemoryData;
		}
		memSectorIndex++;
		curAddr = curAddr + (DWORD_PTR)regionSizeOrg;
		continue;
	}
	return TRUE;
}


int MemoryManager::find(BYTE* buffer, int dwBufferSize, BYTE* bstr, DWORD dwStrLen) {
	if (dwBufferSize < 0) {
		return -1;
	}
	int  i, j;
	for (i = 0; i < dwBufferSize; i++) {
		for (j = 0; j < dwStrLen; j++) {
			if ((buffer[i + j] != bstr[j]) && (bstr[j] != '?'))
				break;
		}
		if (j == dwStrLen)
			return i;
	}
	return -1;
}


DWORD MemoryManager::dRead(DWORD base) {
	readMemory((PVOID)base, &_d, 4);
	return _d;
}
float MemoryManager::fRead(DWORD base) {
	readMemory((PVOID)base, &_f, 4);
	return _f;
}
int MemoryManager::iRead(DWORD base) {
	readMemory((PVOID)base, &_i, 4);
	return _i;
}
uint64 MemoryManager::uiRead(DWORD base) {
	readMemory((PVOID)base, &_u, 8);
	return _u;
}
bool MemoryManager::bRead(DWORD base) {
	readMemory((PVOID)base, &_b, 1);
	return _b;
}
string MemoryManager::charRead(DWORD base)
{
	readMemory((PVOID)base, &_char, 32);
	return &_char;
}

Vector3f MemoryManager::v3Read(DWORD base)
{
	readMemory((PVOID)base, &_v3, 12);
	return _v3;
}
string MemoryManager::charp32Read(DWORD base)
{
	readMemory((PVOID)base, &_playerNameBuffer, 32);
	string playerName = "";
	int i = 0;
	for (i = 0; i < 32; i++)
	{
		if ((_playerNameBuffer[i] == 0x00) && (i > 0) && (i % 2 == 0))
		{
			break;
		}
		if (((i + 1) % 2) == 0)
		{
			continue;
		}
		if (_playerNameBuffer[i] != 0x00)
		{
			if ((_playerNameBuffer[i] == '\n') || (_playerNameBuffer[i] == '\r'))
				playerName += '?';
			else
				playerName += _playerNameBuffer[i];

		}
	}
	return playerName;
}

FTTransform2_t MemoryManager::structRead(DWORD base)
{
	readMemory((PVOID)base, &_struct, 44);
	return _struct;
}
