// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

extern HMODULE g_hModule;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	g_hModule = hModule;
	
	Wrapper::Init();
	
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		EntryPoint();
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

