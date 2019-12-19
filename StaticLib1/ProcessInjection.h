#pragma once
#include "pch.h"

/*
 * Inject using CreateRemoteThread
 */

class ProcessInjecter
{
	HANDLE m_hProcess = NULL;
	LPBYTE m_pLoadLibraryW = NULL;
	LPBYTE m_pPathAlloc = NULL;
	DWORD m_dwPathAllocSize = 0;
	
public:
	bool Install(DWORD dwTargetPid, LPCWSTR wzDllPath);
	bool Run();
	void Uninstall();
};

