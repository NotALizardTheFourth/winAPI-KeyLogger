// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include<stdio.h>

HHOOK gHookHandle;

LRESULT CALLBACK keyLoggerHook(int nCode, WPARAM wParam, LPARAM lParam) {
    printf("Inside hook!");
    return CallNextHookEx(gHookHandle, nCode, wParam, lParam);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        gHookHandle = SetWindowsHookEx(WH_KEYBOARD, keyLoggerHook, hModule, 0);

        if (NULL == gHookHandle) {
            printf("ERROR CREATING HOOK: \n");
            printf("%d\n", GetLastError());
            getchar();
            return 0;
        }

        MSG message;

        while (GetMessage(&message, NULL, 0, 0) != 0) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        printf("Press any key to quit...");
        getchar();

        UnhookWindowsHookEx(gHookHandle);

        return 0;
        break;
    }
    return TRUE;
}