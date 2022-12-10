#pragma once

#include <Windows.h>
#include <string>

#include "Xor.h"

#ifdef DEBUG
#  define DEBUG_PRINT(x) x
#else
#  define DEBUG_PRINT(x) 
#endif

namespace /* Anonymous */
{
	enum class ErrCode
	{
		// Security.cpp
		DbgDetected = 1000,
		AnalysisSoftware,
		HashMemFailed,
		CryptContextFailed,
		CryptCreateHashFailed,
		CryptHashDataFailed,
		CryptGetHashFailed,
		// ClientSocket.cpp
		WSAStartupFailed = 2000,
		GetAddrFailed,
		SocketFailed,
		ShutDownFailed,
		UnrecognizedPacket,
		// Injector.cpp
		InvalidRawFile = 3000,
		InvalidSignature,
		InvalidPlatform,
		MemTargetAllocFailed,
		FailedMapSections,
		MemAllocShellFailed,
		ThreadCreationFailed,
		FailedToGetProcId,
		FailedToOpenProc,
		// Core.cpp
		SecurityEventFailed = 4000,
		SecurityThreadFailed,
		MemAllocGameDataFailed,
		MemAllocClientSocketFailed,
		CoreLogicalError,
		InitPacketFailed,
		ErrorProcessingPacket
	};

	void DisplayError(const ErrCode code)
	{
		switch (code)
		{
		case ErrCode::DbgDetected:
			MessageBox(0, skCrypt("Dbg Software Detected"), skCrypt("ERROR"), MB_ICONERROR);
			break;
		case ErrCode::AnalysisSoftware:
			MessageBox(0, skCrypt("Analysis Software Caught"), skCrypt("ERROR"), MB_ICONERROR);
			break;
		default:
			printf(skCrypt("[ERROR] Report this code: %d\n"), (int)code);
		}
	};

	enum class InfoCode
	{
		// ClientSocket.cpp
		FailedConnServer = 100,
		SuccessConnServer,
		// Models.h (InputLicense Message)
		InputLicenseMsg = 200,
		// Injector.cpp
		InjectionAborted = 300,
		// Core.cpp
		WaitingGameMsg = 400,
		GameNotFound,
		InjectSuccess,
		LoginErrMsg,
		LostConnToServer,
		// main.cpp
		RestartAsAdmin = 500
	};

	void DisplayInfo(const InfoCode code, const char* optMsg = nullptr)
	{
		switch (code)
		{
		case InfoCode::FailedConnServer:
			printf(skCrypt("[INFO] Unable to connect to server!\n"));
			break;
		case InfoCode::SuccessConnServer:
			printf(skCrypt("[INFO] Connected to Server\n"));
			break;
		case InfoCode::InputLicenseMsg:
			printf(skCrypt("[INFO] Please Input Your Licence Key: "));
			break;
		case InfoCode::InjectionAborted:
			printf(skCrypt("[INFO] Injection Aborted !\n"));
			break;
		case InfoCode::WaitingGameMsg:
		{
			if (!optMsg) return;
			printf(skCrypt("\r[INFO] Waiting for the Game: %s sec"), optMsg);
			fflush(stdout);
			break;
		}
		case InfoCode::GameNotFound:
			MessageBox(0, skCrypt("Game not found ! Try Again Later ..."), skCrypt("INFO"), MB_ICONWARNING);
			break;
		case InfoCode::InjectSuccess: 
			MessageBox(0, skCrypt("Injected With Success"), skCrypt("INFO"), MB_ICONINFORMATION);
			break;
		case InfoCode::LoginErrMsg:
		{
			if (!optMsg) return;
			printf(skCrypt("[INFO] [LOGIN] : %s\n"), optMsg);
			break;
		}
		case InfoCode::LostConnToServer:
			printf(skCrypt("[INFO] Lost connection with the server ...\n"));
			break;
		case InfoCode::RestartAsAdmin:
			MessageBox(0, skCrypt("Please restart this program as administrator !"), skCrypt("INFO"), MB_ICONWARNING);
			break;
		default: 
			printf(skCrypt("[INFO] Info code: %d\n"), (int)code);
		}
	}
}