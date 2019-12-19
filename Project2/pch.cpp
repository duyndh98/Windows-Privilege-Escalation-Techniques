// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

HMODULE g_hModule;

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
void WINAPI EntryPoint()
{
	WCHAR wzFilePath[MAX_PATH];
	GetModuleFileNameW(g_hModule, wzFilePath, _countof(wzFilePath));
	PathRenameExtensionW(wzFilePath, L".txt");

	LPBYTE pData = NULL;
	DWORD dwDataSize = 0;

	do
	{
		if (!Wrapper::ReadFromFile(wzFilePath, pData, dwDataSize))
			break;

		Wrapper::CreateNewProcess((LPWSTR)pData);

	} while (false);

	if (pData)
		VirtualFree(pData, dwDataSize, MEM_DECOMMIT);
}

//void WINAPI Inject()
//{
//	// Open
//	HANDLE hProcess = GetCurrentProcess();
//	if (NULL == hProcess)
//		return;
//
//	DWORD dwTid = 0;
//	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)EntryPoint, NULL, 0, &dwTid);
//	if (!hThread)
//		return;
//}