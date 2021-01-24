// RayTracingInNotepad.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <winuser.h>
#include "RayTracingInNotepad.h"
#include <string>
#include <tlhelp32.h>
#include <iostream>
using namespace std;

DWORD ProcessID;

HWND GetWindowForProcessAndClassName(DWORD pid, const char* className)
{
    HWND curWnd = GetTopWindow(0);
    char classNameBuf[256];

    while (curWnd != NULL) {
        DWORD curPid;
        DWORD dwThreadId = GetWindowThreadProcessId(curWnd, &curPid);

        if (curPid == pid) {
            GetClassName(curWnd, (LPWSTR)classNameBuf, 256);
            if (strcmp(className, classNameBuf) == 0) return curWnd;

            HWND childWindow = FindWindowEx(curWnd, NULL, (LPCWSTR)className, NULL);
            if (childWindow != NULL) return childWindow;
        }
        curWnd = GetNextWindow(curWnd, GW_HWNDNEXT);
    }
    return NULL;
}

char* FindBytePatternInProcessMemory(HANDLE process, const char* pattern, size_t patternLen)
{
    char* basePtr = (char*)0x0;

    MEMORY_BASIC_INFORMATION memInfo;

    while (VirtualQueryEx(process, (void*)basePtr, &memInfo, sizeof(MEMORY_BASIC_INFORMATION)))
    {
        const DWORD mem_commit = 0x1000;
        const DWORD page_readwrite = 0x04;
        if (memInfo.State == mem_commit && memInfo.Protect == page_readwrite)
        {
            // search this memory for our pattern
        }

        basePtr = (char*)memInfo.BaseAddress + memInfo.RegionSize;
    }

    return basePtr;
}

bool IsProcessPresent(wchar_t* szExe)
{
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &pe);

    if (!_wcsicmp((wchar_t*)&pe.szExeFile, szExe))
    {
        return true;
    }

    while (Process32Next(hSnapshot, &pe))
    {
        if (!_wcsicmp((wchar_t*)&pe.szExeFile, szExe))
        {
            return true;
        }
    }

    return false;
}

void InitNotepad() {
    TCHAR szExe[] = L"notepad.exe";
    TCHAR szPath[] = TEXT("c:\\windows\\notepad.exe"); // Путь к файлу
    STARTUPINFO si;
    PROCESS_INFORMATION pi; 
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    memset(&pi, 0, sizeof(pi));
    CreateProcess(NULL, szPath, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
    ProcessID = pi.dwProcessId;
}

int main()
{
    setlocale(LC_ALL, "");
    std::cout << "TMP\n";

    InitNotepad();
    HWND t = GetWindowForProcessAndClassName(ProcessID, "Notepad");
    
    PostMessageA(t, WM_CHAR, VkKeyScan('A'), 0);
    std::cin;
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
