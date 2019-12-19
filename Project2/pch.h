// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <windows.h>
#include <Shlwapi.h>
#include <winternl.h>

#include <string>

#include "../StaticLib1/Wrapper.h"
#include "../StaticLib1/ProcessInjection.h"
#pragma comment(lib, "StaticLib1")

#pragma comment(lib, "User32")

#define MUTEX_GUID L"{B4F99EFA-88D9-43AD-9977-88431BC221B8}"
#define LOG_PATH L"%USERPROFILE%\\Desktop\\Demo_PE\\log.txt"

void WINAPI EntryPoint();

#endif //PCH_H
