#pragma once
#include "pch.h"

class SystemPropertiesAdvancedHijacker
{
public:
	bool Install(LPCWSTR wzDllPath, LPCWSTR wzCommand);
	bool Run();
	void Uninstall();
};


