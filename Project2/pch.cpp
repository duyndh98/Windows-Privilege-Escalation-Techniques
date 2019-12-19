// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
void WINAPI EntryPoint()
{
	MessageBoxW(NULL, __FUNCTIONW__, NULL, MB_OK);
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