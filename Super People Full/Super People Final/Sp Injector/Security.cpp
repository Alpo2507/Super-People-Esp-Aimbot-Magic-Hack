#include "Security.h"

BOOL Security::IsProcessRunning(const char* const processName, BOOL isProcessWindowTitle)
{
	if (isProcessWindowTitle)
	{
		DWORD processId{ NULL };
		GetWindowThreadProcessId(FindWindow(nullptr, processName), &processId);

		return processId == NULL ? false : true;
	}

	PROCESSENTRY32 PE32{ 0 };
	PE32.dwSize = sizeof(PE32);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap == INVALID_HANDLE_VALUE)
	{
		return true;
	}

	BOOL bRet = Process32First(hSnap, &PE32);
	while (bRet)
	{
		if (strcmp(PE32.szExeFile, processName) == 0) return true;
		bRet = Process32Next(hSnap, &PE32);
	}

	CloseHandle(hSnap);
	return false;
}

BOOL Security::IsAdministrator(VOID)
{
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID AdministratorsGroup;

	if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdministratorsGroup))
	{
		return FALSE;
	}

	BOOL IsInAdminGroup = FALSE;

	if (!CheckTokenMembership(NULL, AdministratorsGroup, &IsInAdminGroup))
	{
		IsInAdminGroup = FALSE;
	}

	FreeSid(AdministratorsGroup);
	return IsInAdminGroup;
}

BOOL Security::DoesRegKeyValueExists(HKEY hKey, const TCHAR* lpSubKey, const TCHAR* lpValueName, const TCHAR* search_str)
{
	HKEY hkResult = NULL;
	TCHAR lpData[1024] = { 0 };
	DWORD cbData = MAX_PATH;

	if (RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_READ, &hkResult) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(hkResult, lpValueName, NULL, NULL, (LPBYTE)lpData, &cbData) == ERROR_SUCCESS)
		{
			if (StrStrI((PCTSTR)lpData, search_str) != NULL)
			{
				RegCloseKey(hkResult);
				return TRUE;
			}
		}
		RegCloseKey(hkResult);
	}
	return FALSE;
}

BOOL Security::DoesRegKeyExists(HKEY hKey, const TCHAR* lpSubKey)
{
	HKEY hkResult = NULL;
	TCHAR lpData[1024] = { 0 };
	DWORD cbData = MAX_PATH;

	if (RegOpenKeyEx(hKey, lpSubKey, NULL, KEY_READ, &hkResult) == ERROR_SUCCESS)
	{
		RegCloseKey(hkResult);
		return TRUE;
	}

	return FALSE;
}

std::string Security::HashMD5(const char* data)
{
	DWORD cbHash = 16;
	HCRYPTPROV cryptProv;
	HCRYPTHASH cryptHash;
	BYTE hash[16];
	auto hex = skCrypt("0123456789abcdef");

	// Allocate memory to store the Hash and check if it failed
	auto strHash = new(std::nothrow) char[500];
	if (!strHash)
	{
		// Failed:
		DisplayError(ErrCode::HashMemFailed);
		return std::string();
	}

	memset(strHash, '\0', 500);

	if (!CryptAcquireContext(&cryptProv, NULL, MS_DEF_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		DisplayError(ErrCode::CryptContextFailed);
		delete[] strHash;
		return std::string();
	}

	if (!CryptCreateHash(cryptProv, CALG_MD5, 0, 0, &cryptHash))
	{
		DisplayError(ErrCode::CryptCreateHashFailed);

		CryptReleaseContext(cryptProv, 0);
		delete[] strHash;
		return std::string();
	}

	if (!CryptHashData(cryptHash, (BYTE*)data, (DWORD)strlen(data), 0))
	{
		DisplayError(ErrCode::CryptHashDataFailed);
		CryptReleaseContext(cryptProv, 0);
		CryptDestroyHash(cryptHash);

		delete[] strHash;
		return std::string();
	}

	if (!CryptGetHashParam(cryptHash, HP_HASHVAL, hash, &cbHash, 0))
	{
		DisplayError(ErrCode::CryptGetHashFailed);
		CryptReleaseContext(cryptProv, 0);
		CryptDestroyHash(cryptHash);

		delete[] strHash;
		return std::string();
	}

	// Decrypt the hex string
	hex.decrypt();

	for (unsigned i = 0; i < cbHash; i++)
	{
		strHash[i * 2] = hex[hash[i] >> 4];
		strHash[(i * 2) + 1] = hex[hash[i] & 0xF];
	}

	CryptReleaseContext(cryptProv, 0);
	CryptDestroyHash(cryptHash);

	std::string stdStrHash(strHash);
	delete[] strHash;

	return stdStrHash;
}

// generating hardware id on windows systems, c++ 54-7-2017
std::string Security::GetHDDSerial()
{
	DWORD disk_serialINT;
	GetVolumeInformationA(NULL, NULL, NULL, &disk_serialINT, NULL, NULL, NULL, NULL);
	return std::to_string(disk_serialINT);
}

std::string Security::GetMac()
{
	char data[4096];
	ZeroMemory(data, 4096);
	unsigned long  len = 4000;
	PIP_ADAPTER_INFO pinfo = (PIP_ADAPTER_INFO)data;
	char sbuf[20];
	std::string sret = "";

	DWORD ret = GetAdaptersInfo(pinfo, &len);
	if (ret != ERROR_SUCCESS)
	{
		return sret;
	}

	for (int k = 0; k < 5; k++) {
		sprintf_s(sbuf, "%02X-", pinfo->Address[k]);
		sret += sbuf;
	}
	sprintf_s(sbuf, "%02X", pinfo->Address[5]);
	sret += sbuf;

	return(sret);
}

std::string Security::GetHwid()
{
	// Get the HWID and Mac Address
	std::string hd = GetHDDSerial() + ":" + GetMac();
	// Hash these values using MD5
	return HashMD5(hd.c_str());
}

BOOL Security::HaveNotEnoughProc()
{

#ifdef _WIN64
	PULONG ulNumberProcessors = (PULONG)(__readgsqword(0x60) + 0xB8);
#else
	PULONG ulNumberProcessors = (PULONG)(__readfsdword(0x30) + 0x64);
#endif

	// If our proc is only 4cores and 8 threads then we suppose we cannot run the injector
	if (*ulNumberProcessors <= 8)
		return TRUE;
	else
		return FALSE;
}

BOOL Security::HaveNoPowerCap()
{
	SYSTEM_POWER_CAPABILITIES powerCaps;
	BOOL bFound = FALSE;
	if (GetPwrCapabilities(&powerCaps) == TRUE)
	{
		if ((powerCaps.SystemS1 | powerCaps.SystemS2 | powerCaps.SystemS3 | powerCaps.SystemS4) == FALSE)
		{
			bFound = (powerCaps.ThermalControl == FALSE);
		}
	}

	return bFound;
}

BOOL Security::HaveVMsRegistry()
{
	// Virtual box
	if 
	(
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"), skCrypt("Identifier"), skCrypt("VBOX")) ||
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\Description\\System"), skCrypt("SystemBiosVersion"), skCrypt("VBOX")) ||
		DoesRegKeyExists(HKEY_LOCAL_MACHINE, skCrypt("SOFTWARE\\Oracle\\VirtualBox Guest Additions")) ||
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\Description\\System"), skCrypt("VideoBiosVersion"), skCrypt("VIRTUALBOX")) ||
		DoesRegKeyExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\ACPI\\DSDT\\VBOX__"))
	) 
	{
		return TRUE;
	}
	
	// VMWARE
	if
	(
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"), skCrypt("Identifier"), skCrypt("VMWARE")) ||
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 1\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"), skCrypt("Identifier"), skCrypt("VMWARE")) ||
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 2\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"), skCrypt("Identifier"), skCrypt("VMWARE")) ||
		DoesRegKeyExists(HKEY_LOCAL_MACHINE, skCrypt("SOFTWARE\\VMware, Inc.\\VMware Tools"))
	)
	{
		return TRUE;
	}
	
	//QEMU
	if 
	(
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\DEVICEMAP\\Scsi\\Scsi Port 0\\Scsi Bus 0\\Target Id 0\\Logical Unit Id 0"), skCrypt("Identifier"), skCrypt("QEMU")) ||
		DoesRegKeyValueExists(HKEY_LOCAL_MACHINE, skCrypt("HARDWARE\\Description\\System"), skCrypt("SystemBiosVersion"), skCrypt("QEMU"))
	)
	{
		return TRUE;
	}
	
	// KVM
	if 
	(
		DoesRegKeyExists(HKEY_LOCAL_MACHINE, skCrypt("SYSTEM\\ControlSet001\\Services\\netkvm")) ||
		DoesRegKeyExists(HKEY_LOCAL_MACHINE, skCrypt("SYSTEM\\ControlSet001\\Services\\vioscsi")) ||
		DoesRegKeyExists(HKEY_LOCAL_MACHINE, skCrypt("SYSTEM\\ControlSet001\\Services\\VirtIO-FS Service"))
	)
	{
		return TRUE;
	}

	// Wine
	if (DoesRegKeyExists(HKEY_CURRENT_USER, skCrypt("SOFTWARE\\Wine")))
		return TRUE;

	return FALSE;
}

void Security::AntiDbg()
{
	/*
	CheckRemoteDebuggerPresent() is another Win32 Debugging API function;
	it can be used to check if a remote process is being debugged. However,
	we can also use this as another method for checking if our own process
	is being debugged. This API internally calls the NTDLL export
	NtQueryInformationProcess function with the PROCESSINFOCLASS set to
	7 (ProcessDebugPort).
	 */

	BOOL bIsRemoteDbgPresent = FALSE;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &bIsRemoteDbgPresent);

	if (IsDebuggerPresent() || bIsRemoteDbgPresent)
	{
		DisplayError(ErrCode::DbgDetected);
		DeleteSelf();
		exit(-1);
	}
}

void Security::AntiAnalysis()
{
	if
		(
			IsProcessRunning(skCrypt("ollydbg.exe"), false) || // Olly Dbg
			IsProcessRunning(skCrypt("ProcessHacker.exe"), false) || // Process Hacker
			IsProcessRunning(skCrypt("tcpview.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("autoruns.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("autorunsc.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("filemon.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("procmon.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("regmon.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("procexp.exe"), false) || // Part of Sysinternals Suite
			IsProcessRunning(skCrypt("idaq.exe"), false) || // IDA Pro Interactive Disassembler
			IsProcessRunning(skCrypt("idaq64.exe"), false) || // IDA Pro Interactive Disassembler
			IsProcessRunning(skCrypt("ImmunityDebugger.exe"), false) || // ImmunityDebugger
			IsProcessRunning(skCrypt("Wireshark.exe"), false) || // Wireshark packet sniffer
			IsProcessRunning(skCrypt("dumpcap.exe"), false) || // Network traffic dump tool
			IsProcessRunning(skCrypt("HookExplorer.exe"), false) || // Find various types of runtime hooks
			IsProcessRunning(skCrypt("ImportREC.exe"), false) || // Import Reconstructor
			IsProcessRunning(skCrypt("PETools.exe"), false) || // PE Tool
			IsProcessRunning(skCrypt("LordPE.exe"), false) || // Lord PE
			IsProcessRunning(skCrypt("SysInspector.exe"), false) || // ESET SysInspector
			IsProcessRunning(skCrypt("proc_analyzer.exe"), false) || // Part of SysAnalyzer iDefense
			IsProcessRunning(skCrypt("sysAnalyzer.exe"), false) || // Part of SysAnalyzer iDefense
			IsProcessRunning(skCrypt("sniff_hit.exe"), false) || // Part of SysAnalyzer iDefense
			IsProcessRunning(skCrypt("windbg.exe"), false) || // Microsoft WinDbg
			IsProcessRunning(skCrypt("joeboxcontrol.exe"), false) || // Part of Joe Sandbox
			IsProcessRunning(skCrypt("joeboxserver.exe"), false) || // Part of Joe Sandbox
			IsProcessRunning(skCrypt("ResourceHacker.exe"), false) || // Resource Hacker
			IsProcessRunning(skCrypt("x32dbg.exe"), false) || // x32dbg
			IsProcessRunning(skCrypt("x64dbg.exe"), false) || // x64dbg
			IsProcessRunning(skCrypt("Fiddler.exe"), false) || // Fiddler
			IsProcessRunning(skCrypt("httpdebugger.exe"), false) || // Http Debugger
			IsProcessRunning(skCrypt("radare2.exe"), false) || // radare Dbg
			IsProcessRunning(skCrypt("HxD.exe"), false)					   // Hex Editor
			)
	{
		DisplayError(ErrCode::AnalysisSoftware);
		DeleteSelf();
		exit(-1);
	}
}

void Security::AntiVMs() 
{
	if (HaveNotEnoughProc() || HaveNoPowerCap() || HaveVMsRegistry())
	{
		DeleteSelf();
		exit(-1);
	}
}

// Command explained: https://www.codeproject.com/Articles/31454/How-To-Make-Your-Application-Delete-Itself-Immedia
void Security::DeleteSelf()
{
	TCHAR szModuleName[MAX_PATH];
	TCHAR szCmd[2 * MAX_PATH];
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, TEXT(skCrypt("cmd.exe /C ping 127.0.0.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")), szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}