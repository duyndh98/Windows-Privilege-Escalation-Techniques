#include "pch.h"
#include "Wrapper.h"

void Wrapper::Init()
{
	PVOID pOldValue;
	Wow64DisableWow64FsRedirection(&pOldValue);
}

bool Wrapper::CreateRegistryKey(HKEY hRootKey, LPCWSTR wzSubKey)
{
	HKEY hKey;
	LSTATUS status;

	// Create
	status = RegCreateKeyW(hRootKey, wzSubKey, &hKey);
	if (ERROR_SUCCESS != status)
		return false;

	RegCloseKey(hKey);

	return true;
}

bool Wrapper::DeleteRegistryKey(HKEY hKeyRoot, LPCWSTR wzSubkey)
{
	LSTATUS status;

	// Delete
	status = RegDeleteKeyW(hKeyRoot, wzSubkey);
	if (ERROR_SUCCESS != status)
		return false;

	return true;
}

bool Wrapper::SetRegistryStringValue(HKEY hKeyRoot, LPCWSTR wzSubkey, LPCWSTR wzName, LPCWSTR wzValue)
{
	HKEY hKey;
	LSTATUS status;

	// Open
	status = RegOpenKeyExW(hKeyRoot, wzSubkey, 0, KEY_ALL_ACCESS | KEY_WOW64_64KEY, &hKey);
	if (ERROR_SUCCESS != status)
		return false;

	// Set
	status = RegSetValueExW(hKey, wzName, 0, REG_SZ, (LPCBYTE)wzValue, (wzValue ? (wcslen(wzValue) + 1) * sizeof(WCHAR) : 0));
	if (ERROR_SUCCESS != status)
		return false;

	RegCloseKey(hKey);

	return true;
}

bool Wrapper::GetResourceData(DWORD dwResourceId, LPCWSTR wzResourceType, LPBYTE& pData, DWORD& dwDataSize)
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

bool Wrapper::ReadFromFile(LPCWSTR wzFilePath, LPBYTE& pData, DWORD& dwDataSize)
{
	// Expand file path
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzFilePath, wzExpanded, _countof(wzExpanded));
	wzFilePath = wzExpanded;

	// Check file exist
	HANDLE hFile = CreateFileW(wzFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// Get file size
	dwDataSize = GetFileSize(hFile, NULL);
	if (!dwDataSize)
		return false;
	
	// Alloc
	pData = (LPBYTE)VirtualAlloc(NULL, dwDataSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (NULL == pData)
		return false;
	
	// Read
	BOOL bRet = ReadFile(hFile, pData, dwDataSize, &dwDataSize, NULL);
	if (!bRet || !dwDataSize)
		return false;	

	CloseHandle(hFile);

	return true;
}

bool Wrapper::WriteToFile(LPCWSTR wzFilePath, LPBYTE pData, DWORD dwDataSize, bool bAppend)
{
	// Expand file path
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzFilePath, wzExpanded, _countof(wzExpanded));
	wzFilePath = wzExpanded;

	// Create directory
	WCHAR wzDirectoryName[MAX_PATH];
	StrCpyW(wzDirectoryName, wzFilePath);
	PathRemoveFileSpecW(wzDirectoryName);
	CreateDirectoryW(wzDirectoryName, NULL);

	// Create file
	HANDLE hFile = CreateFileW(wzFilePath, (bAppend ? FILE_APPEND_DATA  : GENERIC_WRITE), FILE_SHARE_WRITE, NULL, (bAppend ? OPEN_ALWAYS : CREATE_ALWAYS), FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// Write
	BOOL bRet;
	DWORD dwBytesWritten = 0;;
	bRet = WriteFile(hFile, pData, dwDataSize, &dwBytesWritten, NULL);
	if (!bRet)
		return false;

	CloseHandle(hFile);

	return true;
}

bool Wrapper::ExecuteShellCommand(LPCWSTR wzCommand)
{
	// Expand command
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzCommand, wzExpanded, _countof(wzExpanded));
	wzCommand = wzExpanded;

	// Execute
	HINSTANCE hRet = ShellExecuteW(NULL, NULL, wzCommand, NULL, NULL, 0);
	if ((int)hRet <= 32)
		return false;
}

bool Wrapper::CreateNewProcess(LPCWSTR wzCommand)
{
	// Expand command
	WCHAR wzExpanded[MAX_PATH];
	ExpandEnvironmentStringsW(wzCommand, wzExpanded, _countof(wzExpanded));
	wzCommand = StrDupW(wzExpanded);

	// Prepare
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	// Create
	BOOL bRet = CreateProcessW(NULL, 
		(LPWSTR)wzCommand,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);

	// Free
	free((LPWSTR)wzCommand);

	return bRet != 0;
}

bool Wrapper::CreateNewService(LPCWSTR wzName, LPCWSTR wzDisplayName, LPCWSTR wzCommand)
{
	WCHAR wzPath[MAX_PATH];
	DWORD dwRet = GetModuleFileNameW(NULL, wzPath, MAX_PATH);
	if (!dwRet)
		return false;

	SC_HANDLE schSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == schSCManager)
		return false;

	SC_HANDLE schService = CreateServiceW(
		schSCManager,
		wzName,
		wzDisplayName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START,
		SERVICE_ERROR_NORMAL,
		wzCommand,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	if (NULL == schService)
		return false;

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return true;
}

bool Wrapper::CreateNewThread(LPBYTE pFunction, LPBYTE pParameter)
{
	return CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)pFunction, pParameter, 0, 0);
		
}

bool Wrapper::FindProcess(DWORD* aFoundPids, DWORD &dwFoundPidsCount, LPCWSTR wzProcessNameFilter, MANDATORY_LEVEL mandatoryLevel)
{
	// Max process listed
	DWORD aRunningPids[1 << 10];
	DWORD dwSizeNeeded = 0;

	// Enum
	if (!EnumProcesses(aRunningPids, sizeof(aRunningPids), &dwSizeNeeded))
		return false;

	// Prepare
	DWORD dwProcessCount = dwSizeNeeded / sizeof(DWORD);
	DWORD dwFoundPidsLimit = dwFoundPidsCount;
	dwFoundPidsCount = 0;

	// Browse all process listed above
	for (size_t iProcess = 0; iProcess < dwProcessCount; iProcess++)
	{
		// Gained limit process found
		if (dwFoundPidsCount >= dwFoundPidsLimit)
			break;

		// Open process
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aRunningPids[iProcess]);
		if (NULL != hProcess)
		{
			bool bSelected = true;
			
			// Compare
			WCHAR wzFileName[MAX_PATH];
			if (wzProcessNameFilter)
			{
				GetModuleFileNameExW(hProcess, NULL, wzFileName, _countof(wzFileName));
				// Matched
				if (NULL == StrStrIW(wzFileName, wzProcessNameFilter))
					bSelected = false;
			}
			
			DWORD dwIntegrityLevel = 0;
			if (-1 != mandatoryLevel && bSelected)
			{
				LPBYTE pData = (LPBYTE)&dwIntegrityLevel;
				DWORD dwDataSize = sizeof(dwIntegrityLevel);
				if (!GetProcessInfo(hProcess, TokenIntegrityLevel, pData, dwDataSize))
					bSelected = false;

				switch (dwIntegrityLevel)
				{
				case SECURITY_MANDATORY_PROTECTED_PROCESS_RID:
					dwIntegrityLevel = MandatoryLevelSecureProcess;
					break;
				case SECURITY_MANDATORY_SYSTEM_RID:
					dwIntegrityLevel = MandatoryLevelSystem;
					break;
				case SECURITY_MANDATORY_HIGH_RID:
					dwIntegrityLevel = MandatoryLevelHigh;
					break;
				case SECURITY_MANDATORY_MEDIUM_PLUS_RID:
					dwIntegrityLevel = MandatoryLevelMedium;
					break;
				case SECURITY_MANDATORY_MEDIUM_RID:
					dwIntegrityLevel = MandatoryLevelMedium;
					break;
				case SECURITY_MANDATORY_LOW_RID:
					dwIntegrityLevel = MandatoryLevelLow;
					break;
				default:
					dwIntegrityLevel = MandatoryLevelUntrusted;
				}
				
				if (dwIntegrityLevel != mandatoryLevel)
					bSelected = false;
			}

			if (bSelected)
				aFoundPids[dwFoundPidsCount++] = aRunningPids[iProcess];

			CloseHandle(hProcess);
		}
	}
	
	return true;
}

bool Wrapper::GetProcessInfo(HANDLE hProcess, TOKEN_INFORMATION_CLASS infoClass, LPBYTE pData, DWORD &dwDataSize)
{
	bool bResult = false;
	LPBYTE pTokenInfo = NULL;
	
	do
	{
		// Open
		HANDLE hToken = NULL;
		BOOL bRet = OpenProcessToken(hProcess, TOKEN_READ, &hToken);
		if (!bRet)
			break;
	
		// Get data size needed
		DWORD dwSizeNeeded = 0;
		bRet = GetTokenInformation(hToken, infoClass, NULL, 0, &dwSizeNeeded);

		pTokenInfo = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwSizeNeeded);
		if (NULL == pTokenInfo)
			break;

		// Get data
		bRet = GetTokenInformation(hToken, infoClass, pTokenInfo, dwSizeNeeded, &dwSizeNeeded);
		if (!bRet)
			break;
		
		if (TokenOwner == infoClass)
		{
			// Prepare
			WCHAR wzName[MAX_PATH];
			WCHAR wzDomainName[MAX_PATH];

			DWORD dwSize = _countof(wzName);
			SID_NAME_USE sidType;

			// Look up		
			bRet = LookupAccountSidW(NULL, ((PTOKEN_OWNER)pTokenInfo)->Owner, wzName, &dwSize, wzDomainName, &dwSize, &sidType);
			if (!bRet)
				break;

			// Pack
			if (wcslen(wzDomainName) + wcslen(wzName) * sizeof(WCHAR) + 2 * sizeof(WCHAR) > dwDataSize)
				break;

			LPWSTR pCurrent = (LPWSTR)pData;

			StrCpyW(pCurrent, wzDomainName);
			pCurrent += wcslen(wzDomainName);
			*pCurrent++ = '\\';

			StrCpyW(pCurrent, wzName);
			pCurrent += wcslen(wzName) + 1;

			dwDataSize = (LPBYTE)pCurrent - (LPBYTE)pData;
		}
		if (TokenSessionId == infoClass)
		{
			if (dwDataSize < sizeof(DWORD))
				break;

			*(LPDWORD)pData = *(LPDWORD)pTokenInfo;
			dwDataSize = 4;
		}
		if (TokenIntegrityLevel == infoClass)
		{
			if (dwDataSize < sizeof(DWORD))
				break;

			PTOKEN_MANDATORY_LABEL pLabel = (PTOKEN_MANDATORY_LABEL)pTokenInfo;
			
			*(LPDWORD)(pData) = *GetSidSubAuthority(pLabel->Label.Sid, *GetSidSubAuthorityCount(pLabel->Label.Sid) - 1);
			
			dwDataSize = 4;
		}
		if (TokenPrivileges == infoClass)
		{
			PTOKEN_PRIVILEGES pPrivileges = (PTOKEN_PRIVILEGES)pTokenInfo;
			
			LPWSTR pCurrent = (LPWSTR)pData;
			WCHAR wzPrivName[MAX_PATH];
			size_t iPriv = 0;
			for (; iPriv < pPrivileges->PrivilegeCount; iPriv++)
			{
				DWORD dwPrivNameLength = _countof(wzPrivName);
				LookupPrivilegeNameW(NULL, &pPrivileges->Privileges[iPriv].Luid, wzPrivName, &dwPrivNameLength);

				if ((LPBYTE)pCurrent - (LPBYTE)pData + wcslen(wzPrivName) * sizeof(WCHAR) + 3 * sizeof(WCHAR) > dwDataSize)
					break;

				StrCpyW(pCurrent, wzPrivName);
				pCurrent += wcslen(wzPrivName);

				StrCpyW(pCurrent, L"\r\n");
				pCurrent += 2;
			}

			if (iPriv < pPrivileges->PrivilegeCount)
				break;
			
			pCurrent++;			
			dwDataSize = (LPBYTE)pCurrent - (LPBYTE)pData;			
		}
	
		bResult = true;
		
	} while (false);
	
	HeapFree(GetProcessHeap(), 0, pTokenInfo);

	return bResult;
}

bool Wrapper::CheckProcess64(HANDLE hProcess, bool& b64Process)
{
	USHORT uProcessMachine = 0;
	USHORT uNativeMachine = 0;

	if (!IsWow64Process2(hProcess, &uProcessMachine, &uNativeMachine))
		return false;

	b64Process = (IMAGE_FILE_MACHINE_UNKNOWN == uProcessMachine && IMAGE_FILE_MACHINE_AMD64 == uNativeMachine);

	return true;
}

bool Wrapper::GetFullProcessInfo(HANDLE hProcess, LPWSTR wzInfo, DWORD dwInfoLength)
{
	DWORD dwRet = 0;
	BOOL bRet = 0;

	WCHAR wzName[MAX_PATH * MAX_PATH];
	LPBYTE pData = NULL;
	DWORD dwDataSize = 0;

	std::wstring wstrInfo = L"\r\n\r\n====================\r\n\r\n";
	
	// Get commandline
	typedef NTSTATUS(WINAPI* FN_NtQueryInformationProcess)(
		IN HANDLE           ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID           ProcessInformation,
		IN ULONG            ProcessInformationLength,
		OUT PULONG          ReturnLength
		);

	FN_NtQueryInformationProcess fnNtQueryInformationProcess =
		(FN_NtQueryInformationProcess)GetProcAddress(GetModuleHandleW(L"ntdll.dll"), "NtQueryInformationProcess");

	PROCESS_BASIC_INFORMATION scProcessBasicInfo;
	DWORD dwProcessBasicInfoSize = sizeof(scProcessBasicInfo);
	if (0 != fnNtQueryInformationProcess(hProcess, ProcessBasicInformation, &scProcessBasicInfo, dwProcessBasicInfoSize, &dwProcessBasicInfoSize))
		return false;

	LPWSTR wzCommandLine = scProcessBasicInfo.PebBaseAddress->ProcessParameters->CommandLine.Buffer;
	wstrInfo.append(L"COMMAND LINE: ");
	wstrInfo.append(wzCommandLine);
	wstrInfo.append(L"\r\n\r\n");
	
	// Get user	
	ZeroMemory(wzName, sizeof(wzName));
	pData = (LPBYTE)wzName;
	dwDataSize = sizeof(wzName);
	bRet = GetProcessInfo(hProcess, TokenOwner, pData, dwDataSize);
	if (!bRet)
		return false;
	wstrInfo.append(L"USER: ");
	wstrInfo.append(wzName);
	wstrInfo.append(L"\r\n\r\n");

	// Get session id
	DWORD dwSessionId = 0;
	pData = (LPBYTE)&dwSessionId;
	dwDataSize = sizeof(dwSessionId);
	bRet = GetProcessInfo(hProcess, TokenSessionId, pData, dwDataSize);
	if (!bRet)
		return false;
	wstrInfo.append(L"SESSSION ID: ");
	wstrInfo.append(std::to_wstring(dwSessionId));
	wstrInfo.append(L"\r\n\r\n");

	// Get intergrity level
	DWORD dwIntegrityLevel = 0;
	pData = (LPBYTE)&dwIntegrityLevel;
	dwDataSize = sizeof(dwIntegrityLevel);
	bRet = GetProcessInfo(hProcess, TokenIntegrityLevel, pData, dwDataSize);
	if (!bRet)
		return false;

	switch (dwIntegrityLevel)
	{
	case SECURITY_MANDATORY_PROTECTED_PROCESS_RID:
		StrCpyW(wzName, L"Protected");
		break;
	case SECURITY_MANDATORY_SYSTEM_RID:
		StrCpyW(wzName, L"System");
		break;
	case SECURITY_MANDATORY_HIGH_RID:
		StrCpyW(wzName, L"High");
		break;
	case SECURITY_MANDATORY_MEDIUM_PLUS_RID:
		StrCpyW(wzName, L"Medium Plus");
		break;
	case SECURITY_MANDATORY_MEDIUM_RID:
		StrCpyW(wzName, L"Medium");
		break;
	case SECURITY_MANDATORY_LOW_RID:
		StrCpyW(wzName, L"Low");
		break;
	default:
		StrCpyW(wzName, L"Untrusted");
	}
				
	wstrInfo.append(L"INTEGRITY: ");
	wstrInfo.append(wzName);
	wstrInfo.append(L"\r\n\r\n");

	// Get privileges
	ZeroMemory(wzName, sizeof(wzName));
	pData = (LPBYTE)wzName;
	dwDataSize = sizeof(wzName);
	bRet = GetProcessInfo(hProcess, TokenPrivileges, pData, dwDataSize);
	if (!bRet)
		return false;
	wstrInfo.append(L"PRIVILEGES:\r\n");
	wstrInfo.append(wzName);
	wstrInfo.append(L"\r\n\r\n");
	
	// Not enough space
	if (wstrInfo.length() >= dwInfoLength)
		return false;
	
	// Copy
	StrCpyW(wzInfo, wstrInfo.c_str());

	return true;
}

void Wrapper::Log(LPCWSTR wzLogPath)
{
	// Get full process info
	WCHAR wzInfo[MAX_PATH * 10] = {0};
	BOOL bRet = Wrapper::GetFullProcessInfo(GetCurrentProcess(), wzInfo, _countof(wzInfo));
	if (!bRet)
		return;

	// Get process name
	WCHAR wzName[MAX_PATH] = {0};
	DWORD dwRet = GetModuleFileNameW(NULL, wzName, _countof(wzName));
	PathStripPathW(wzName);

	// Show messagebox
	//MessageBoxW(NULL, wzInfo, wzName, MB_OK);
	bRet = Wrapper::WriteToFile(wzLogPath, (LPBYTE)wzInfo, wcslen(wzInfo) * sizeof(WCHAR), true);
	if (!bRet)
		return;
}
