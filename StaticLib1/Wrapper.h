#pragma once
#include "pch.h"

class Wrapper
{
public:
	static void Init();

	static bool CreateRegistryKey(HKEY hRootKey, LPCWSTR wzSubKey);
	static bool DeleteRegistryKey(HKEY hKeyRoot, LPCWSTR wzSubkey);
	static bool SetRegistryStringValue(HKEY hKeyRoot, LPCWSTR wzSubkey, LPCWSTR wzName, LPCWSTR wzValue);

	static bool GetResourceData(DWORD dwResourceId, LPCWSTR wzResourceType, LPBYTE& pData, DWORD& dwDataSize);
	static bool WriteToFile(LPCWSTR wzFilePath, LPBYTE pData, DWORD dwData);
};

