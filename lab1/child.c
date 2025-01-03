#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#pragma execution_character_set("utf-8")

int main() {
    wprintf(L"Child: ������ ������ ��������� ��������\n");

    wchar_t fileName[MAX_PATH];
    DWORD read;

    wprintf(L"Child: ������ ������ �� pipe...\n");
    char buffer[MAX_PATH];
    if (!ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, sizeof(buffer) - 1, &read, NULL)) {
        fwprintf(stderr, L"Child: ������ ������ �� pipe\n");
        return 1;
    }
    buffer[read] = '\0';

    wprintf(L"Child: �������� ������ (%lu ����):\n", read);
    for (DWORD i = 0; i < read; i++) {
        fwprintf(stderr, L"Child: Byte[%lu]: %02x\n", i, ((unsigned char *)buffer)[i]);
    }

    wprintf(L"Child: �������������� � wchar_t...\n");
    if (MultiByteToWideChar(CP_UTF8, 0, buffer, -1, fileName, MAX_PATH) == 0) {
        fwprintf(stderr, L"Child: ������ �������������� ������� ������\n");
        return 1;
    }

    size_t len = wcslen(fileName);
    while (len > 0 && (fileName[len - 1] == L'\r' || fileName[len - 1] == L'\n' || fileName[len - 1] == L' ')) {
        fileName[--len] = L'\0';
    }

    wprintf(L"Child: ���������� ��� �����: '%ls' (�����: %llu)\n", fileName, len);

    if (GetFileAttributesW(fileName) == INVALID_FILE_ATTRIBUTES) {
        fwprintf(stderr, L"Child: ���� �� ���������� ��� ����������: '%ls'\n", fileName);
        return 1;
    }

    wprintf(L"Child: �������� �����...\n");
    FILE *file = _wfopen(fileName, L"r");
    if (!file) {
        fwprintf(stderr, L"Child: ������ �������� �����: '%ls'\n", fileName);
        perror("Child: ������ �������");
        return 1;
    }

    wprintf(L"Child: ������ ������ �� �����...\n");
    float number, sum = 0;
    while (fwscanf(file, L"%f", &number) == 1) {
        sum += number;
    }
    fclose(file);

    wprintf(L"Child: ����� �����: %.2f\n", sum);
    wprintf(L"Child: ���������� ������ ��������� ��������\n");
    return 0;
}
