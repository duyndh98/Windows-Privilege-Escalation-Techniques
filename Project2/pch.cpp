// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

HMODULE g_hModule;

void WINAPI EntryPoint()
{
	Wrapper::WriteToFile(LOG_PATH, (LPBYTE)__FUNCTIONW__, wcslen(__FUNCTIONW__) * sizeof(WCHAR), true);
	
	// Get integrity
	DWORD dwIntegrityLevel = 0;
	LPBYTE pData = (LPBYTE)&dwIntegrityLevel;
	DWORD dwDataSize = sizeof(dwIntegrityLevel);
	bool bRet = Wrapper::GetProcessInfo(GetCurrentProcess(), TokenIntegrityLevel, pData, dwDataSize);
	if (!bRet)
		return;

	// Build mutex name
	WCHAR wzMutexName[MAX_PATH];
	StrCpyW(wzMutexName, MUTEX_GUID);
	StrCpyW(wzMutexName + wcslen(wzMutexName), L"_");
	_ltow_s(dwIntegrityLevel, wzMutexName + wcslen(wzMutexName), _countof(wzMutexName) - wcslen(wzMutexName), 10);

	// Check singleton instance
	CreateMutexW(NULL, TRUE, wzMutexName);
	if (ERROR_ALREADY_EXISTS == GetLastError())
		return;
	
	Wrapper::Log(LOG_PATH);
	
	//if (dwIntegrityLevel < SECURITY_MANDATORY_SYSTEM_RID)
	//{
	//	WCHAR wzName[MAX_PATH] = {0};
	//	GetModuleFileNameW(g_hModule, wzName, _countof(wzName));

	//	MessageBoxW(NULL, wzName, NULL, MB_OK);

	//	DWORD aPidsFound[1];
	//	DWORD dwPidsFound = _countof(aPidsFound);
	//	//Wrapper::FindProcess(aPidsFound, dwPidsFound, L"TeamViewer_Service.exe", SECURITY_MANDATORY_SYSTEM_RID);		
	//}
}