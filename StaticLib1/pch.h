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
#include <Shlwapi.h>


#pragma comment(lib, "Shlwapi")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "Shell32")

#endif //PCH_H
