// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

void WINAPI EntryPoint()
{
	MessageBoxW(NULL, __FUNCTIONW__, NULL, MB_OK);
}