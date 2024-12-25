#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fcntl.h>
#include <io.h>

#pragma execution_character_set("utf-8")

int main() {
    // ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);

    wprintf(L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜\n");

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};
    wchar_t fileName[MAX_PATH];

    wprintf(L"Main: ˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜: ");
    fgetws(fileName, MAX_PATH, stdin);

    // ˜˜˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ (\r, \n, ˜˜˜˜˜˜˜˜)
    size_t len = wcslen(fileName);
    while (len > 0 && (fileName[len - 1] == L'\r' || fileName[len - 1] == L'\n' || fileName[len - 1] == L' ')) {
        fileName[--len] = L'\0';
    }

    wprintf(L"Main: ˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜: '%ls' (˜˜˜˜˜: %llu)\n", fileName, len);

    // ˜˜˜˜˜˜˜˜ pipe
    wprintf(L"Main: ˜˜˜˜˜˜˜˜ pipe...\n");
    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        fwprintf(stderr, L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜ pipe\n");
        return 1;
    }

    STARTUPINFO si = {sizeof(STARTUPINFO)};
    PROCESS_INFORMATION pi = {0};
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdInput = hReadPipe;

    wprintf(L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜...\n");
    if (!CreateProcessW(NULL, L"child.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fwprintf(stderr, L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜\n");
        CloseHandle(hReadPipe);
        CloseHandle(hWritePipe);
        return 1;
    }

    wprintf(L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜ ˜ pipe...\n");
    DWORD written;
    if (!WriteFile(hWritePipe, fileName, (len + 1) * sizeof(wchar_t), &written, NULL)) {
        fwprintf(stderr, L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ pipe\n");
        CloseHandle(hWritePipe);
        CloseHandle(hReadPipe);
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return 1;
    }

    wprintf(L"Main: ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜ pipe...\n");
    CloseHandle(hWritePipe);

    wprintf(L"Main: ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜...\n");
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode;
    if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
        wprintf(L"Main: ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜ ˜˜˜˜˜: %lu\n", exitCode);
    } else {
        fwprintf(stderr, L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜\n");
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hReadPipe);

    wprintf(L"Main: ˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜. ˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜...\n");
    getchar();

    return 0;
}
