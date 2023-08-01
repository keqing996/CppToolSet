#pragma once

#include <string>

namespace Grf::UtilWin32
{
	std::wstring WinGetUserName();
	bool WinCreateProcess(std::wstring cmdLine);
}