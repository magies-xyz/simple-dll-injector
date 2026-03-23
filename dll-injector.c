#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: dll-injector.exe <path-to-dll> <PID>\n");
        return 1;
    }

    const char* dll_path = argv[1];
    DWORD PID = (DWORD)atoi(argv[2]);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
    if (hProcess == NULL) {
        printf("Failed to retrieve handle to remote process: %lu\n", GetLastError());
        return 1;
    }

    LPVOID allocated_mem = VirtualAllocEx(
        hProcess,
        NULL,
        strlen(dll_path) + 1,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (allocated_mem == NULL) {
        printf("Failed to allocate memory in remote process: %lu\n", GetLastError());
        return 1;
    }

    printf("Memory allocated at: %p\n", allocated_mem);

    if (!WriteProcessMemory(
            hProcess,
            allocated_mem,
            dll_path,
            strlen(dll_path) + 1,
            NULL)) {
        printf("WriteProcessMemory failed: %lu\n", GetLastError());
        return 1;
    }

    HMODULE kernel32Base = GetModuleHandleA("kernel32.dll");
    if (kernel32Base == NULL) {
        printf("Failed to get kernel32.dll: %lu\n", GetLastError());
        return 1;
    }

    FARPROC load_library_address = GetProcAddress(kernel32Base, "LoadLibraryA");

    HANDLE hThread = CreateRemoteThread(
        hProcess,
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)load_library_address,
        allocated_mem,
        0,
        NULL
    );

    if (hThread == NULL) {
        printf("Failed to create remote thread: %lu\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    DWORD exitCode;
    GetExitCodeThread(hThread, &exitCode);

    printf("Remote thread exit code: %lu\n", exitCode);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return 0;
}
