#include "pch.h"
#include "Lib.h"

void Lib::Init()
{
	PVOID pOldValue;
	Wow64DisableWow64FsRedirection(&pOldValue);
}

bool Lib::CreateRegistryKey(HKEY hRootKey, LPCWSTR wzSubKey)
{
	HKEY hKey;
	LSTATUS status;
	
	status = RegCreateKeyW(hRootKey, wzSubKey, &hKey);
	if (ERROR_SUCCESS != status)
		return false;
	
	RegCloseKey(hKey);

	return true;
}

bool Lib::DeleteRegistryKey(HKEY hKeyRoot, LPCWSTR wzSubkey)
{
	LSTATUS status;
	
	status = RegDeleteKeyW(hKeyRoot, wzSubkey);
	if (ERROR_SUCCESS != status)
		return false;

	return true;
}

bool Lib::SetRegistryStringValue(HKEY hKeyRoot, LPCWSTR wzSubkey, LPCWSTR wzName, LPCWSTR wzValue)
{
	HKEY hKey;
	LSTATUS status;
	
	status = RegOpenKeyExW(hKeyRoot, wzSubkey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	if (ERROR_SUCCESS != status)
		return false;
	
	status = RegSetValueExW(hKey, wzName, 0, REG_SZ, (LPCBYTE)wzValue,  (wzValue ? (wcslen(wzValue) + 1) * sizeof(WCHAR) : 0));
	if (ERROR_SUCCESS != status)
		return false;

	RegCloseKey(hKey);
	
	return true;
}

bool Lib::GetResourceData(DWORD dwResourceId, LPCWSTR wzResourceType, LPBYTE &pData, DWORD &dwDataSize)
{
	HMODULE hModule = GetModuleHandleW(NULL);
	if (!hModule)
		return false;
	
	HRSRC hResource = FindResourceW(hModule, MAKEINTRESOURCE(dwResourceId), wzResourceType);
	if (!hResource)
		return false;
	
	HGLOBAL hMemory = LoadResource(hModule, hResource);
	if (!hMemory)
		return false;
	
	dwDataSize = SizeofResource(hModule, hResource);
	if (!dwDataSize)
		return false;
	
	pData = (LPBYTE)LockResource(hMemory);
	if (!pData)
		return false;

	return true;
}

bool Lib::WriteToFile(LPCWSTR wzFilePath, LPBYTE pData, DWORD dwDataSize)
{
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzFilePath, wzExpanded, _countof(wzExpanded));
	wzFilePath = wzExpanded;

	WCHAR wzDirectoryName[MAX_PATH];
	StrCpyW(wzDirectoryName, wzFilePath);
	PathRemoveFileSpecW(wzDirectoryName);
	CreateDirectoryW(wzDirectoryName, NULL);
	
	HANDLE hFile = CreateFileW(wzFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	BOOL bRet;
	DWORD dwBytesWritten = 0;;
	bRet = WriteFile(hFile, pData, dwDataSize, &dwBytesWritten, NULL);
	if (!bRet)
		return false;

	CloseHandle(hFile);

	return true;
}
