#include <windows.h>
#include <stdio.h>

DWORD WINAPI MainThread(LPVOID param) {
    char message[256];
    char processName[MAX_PATH];
    DWORD pid = GetCurrentProcessId();
    GetModuleFileNameA(NULL, processName, MAX_PATH);
    snprintf(message, sizeof(message),
             "I'm loaded in %lu aka %s",
             pid, processName);
    MessageBoxA(NULL, message, "Injected DLL", MB_OK | MB_ICONINFORMATION);
    return 0;
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        CreateThread(NULL, 0, MainThread, NULL, 0, NULL);
    }
    return TRUE;
}
