#pragma once
#include "pch.h"

#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "Shell32")

class Lib
{
public:
	static void Init();
	
	static bool CreateRegistryKey(HKEY hRootKey, LPCWSTR wzSubKey);
	static bool DeleteRegistryKey(HKEY hRootKey, LPCWSTR wzSubkey);
	static bool SetRegistryStringValue(HKEY hRootKey, LPCWSTR wzSubkey, LPCWSTR wzName, LPCWSTR wzValue);

	static bool GetResourceData(DWORD dwResourceId, LPCWSTR wzResourceType, LPBYTE& pData, DWORD& dwDataSize);
	static bool WriteToFile(LPCWSTR wzFilePath, LPBYTE pData, DWORD dwData);
};

