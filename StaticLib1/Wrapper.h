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
	static bool ReadFromFile(LPCWSTR wzFilePath,LPBYTE &pData, DWORD & dwDataSize);
	static bool WriteToFile(LPCWSTR wzFilePath, LPBYTE pData, DWORD dwData, bool bAppend);

	static bool ExecuteShellCommand(LPCWSTR wzCommand);
	static bool CreateNewProcess(LPCWSTR wzCommand);
	static bool CreateNewService(LPCWSTR wzName, LPCWSTR wzDisplayName, LPCWSTR wzCommand);
	static bool CreateNewThread(LPBYTE pFunction, LPBYTE pParameter);

	static bool FindProcess(DWORD* aFoundPids, DWORD& dwFoundPidsCount, LPCWSTR wzProcessNameFilter, MANDATORY_LEVEL mandatoryLevel);

	static bool GetProcessInfo(HANDLE hProcess, TOKEN_INFORMATION_CLASS infoClass, LPBYTE pData, DWORD &dwDataSize);
	static bool CheckProcess64(HANDLE hProcess, bool &b64Process);
	static bool GetFullProcessInfo(HANDLE hProcess, LPWSTR wzInfo, DWORD dwInfoLength);

	static void Log(LPCWSTR wzLogPath);
};