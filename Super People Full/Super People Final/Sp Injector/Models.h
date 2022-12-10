#pragma once

#include <Windows.h>
#include <string>
#include <iostream>

#include "Security.h"

namespace Models
{
	struct LicenseData
	{
		std::string license_key{};
		std::string hwid{};

		static int InputLicenseData(LicenseData& data)
		{
			HANDLE hStdout;
			hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hStdout == INVALID_HANDLE_VALUE) return -1;

			DisplayInfo(InfoCode::InputLicenseMsg);

			std::getline(std::cin, data.license_key);
			data.hwid = Security::GetHwid();

			// Check for errors
			if (data.hwid.empty()) return -1;

			return 0;
		}
	};

	struct GameData
	{
		std::string process_name{};
		bool is_process_window{ false };
	};
}
