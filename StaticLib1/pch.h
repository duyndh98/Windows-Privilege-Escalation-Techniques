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
#include <strsafe.h>
#include <winscard.h>
#include <shellapi.h>
#include <Shlwapi.h>
#include <psapi.h>
#include <winternl.h>

#include <string>
#include <map>

#pragma comment(lib, "Shlwapi")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "Shell32")
#pragma comment(lib, "User32.lib")

#define LOG_PATH L"%USERPROFILE%\\Desktop\\Demo_PE\\log.txt"

#endif //PCH_H
