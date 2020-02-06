#pragma once

#include <Windows.h>
#include <string>

namespace MemoryUtils
{
	DWORD FindBytePattern(std::string moduleName, std::string pattern);
	DWORD FindIDAPattern(std::string moduleName, BYTE* Mask, char* szMask);
}