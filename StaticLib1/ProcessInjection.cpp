#include "pch.h"

#include "Wrapper.h"
#include "ProcessInjection.h"

bool ProcessInjecter::Install(DWORD dwTargetPid, LPCWSTR wzDllPath)
{
	// Expand path
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzDllPath, wzExpanded, _countof(wzExpanded));
	wzDllPath = wzExpanded;

	// Open
	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTargetPid);
	if (NULL == m_hProcess)
		return false;

	// Get LoadLibraryW address
	m_pLoadLibraryW = (LPBYTE)GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "LoadLibraryW");
	if (NULL == m_pLoadLibraryW)
		return false;

	// Allocate dll path in target process
	m_dwPathAllocSize = (wcslen(wzDllPath) + 1) * sizeof(WCHAR);
	m_pPathAlloc = (LPBYTE)VirtualAllocEx(m_hProcess, NULL, m_dwPathAllocSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (NULL == m_pPathAlloc)
		return false;

	// Write dll path to alloc space
	BOOL bRet = WriteProcessMemory(m_hProcess, m_pPathAlloc, wzDllPath, m_dwPathAllocSize, NULL);
	if (0 == bRet)
		return false;

	return true;
}

bool ProcessInjecter::Run()
{
	// Check
	if (!m_hProcess || !m_pLoadLibraryW || !m_pPathAlloc)
		return false;

	// Load dll
	DWORD dwTid = 0;
	HANDLE hThread = CreateRemoteThread(m_hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)m_pLoadLibraryW, (LPVOID)m_pPathAlloc, 0, &dwTid);
	if (!hThread)
		return false;
	
	return true;
}

void ProcessInjecter::Uninstall()
{
	if (m_hProcess && m_pLoadLibraryW && m_pPathAlloc)
	{
		// Clean up	
		VirtualFreeEx(m_hProcess, m_pPathAlloc, m_dwPathAllocSize, MEM_DECOMMIT);		
		
		m_hProcess = m_pPathAlloc = NULL;
		m_dwPathAllocSize = 0;
		
		CloseHandle(m_hProcess);
	}
}
