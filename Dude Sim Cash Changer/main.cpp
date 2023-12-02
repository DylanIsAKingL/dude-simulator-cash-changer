// Dude Sim Cash Changer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>
#include <stdlib.h>

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

DWORD GetPointerAddress(HWND hwnd, DWORD gameBaseAddr, DWORD address, std::vector<DWORD> offsets)
{
    DWORD pID = NULL; // Game process ID
    GetWindowThreadProcessId(hwnd, &pID);
    HANDLE phandle = NULL;
    phandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (phandle == INVALID_HANDLE_VALUE || phandle == NULL);

    DWORD offset_null = NULL;
    ReadProcessMemory(phandle, (LPVOID*)(gameBaseAddr + address), &offset_null, sizeof(offset_null), 0);
    DWORD pointeraddress = offset_null; // the address we need
    for (int i = 0; i < offsets.size() - 1; i++) // we dont want to change the last offset value so we do -1
    {
        ReadProcessMemory(phandle, (LPVOID*)(pointeraddress + offsets.at(i)), &pointeraddress, sizeof(pointeraddress), 0);
    }
    return pointeraddress += offsets.at(offsets.size() - 1); // adding the last offset
}

int main()
{
    SetConsoleTitle(_T("Dude Simulator Cash Changer - https://discord.gg/PfYMfbzMj7"));

    HWND window = FindWindowA(NULL, "Bla Bla");

    while (!window)
    {
        std::cout << "Dude simulator window not found, trying again in 3 seconds...";
        Sleep(1000);
        system("CLS");
        std::cout << "Dude simulator window not found, trying again in 2 seconds...";
        Sleep(1000);
        system("CLS");
        std::cout << "Dude simulator window not found, trying again in 1 seconds...";
        Sleep(1000);
        system("CLS");

        window = FindWindowA(NULL, "Bla Bla");
    }

    DWORD procId = 0;
    GetWindowThreadProcessId(window, &procId);

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, true, procId);

    if (process == INVALID_HANDLE_VALUE || process == NULL)
    {
        std::cout << "Couldnt find handle";
        Sleep(10000);
        return 0;
    }

    char gameModule[] = "DudeSimulator.exe";
    DWORD client = GetModuleBaseAddress(_T(gameModule), procId);

    DWORD cashAddr = 0x103BF58;
    std::vector<DWORD> cashOffsets{ 0x28, 0x10, 0x4, 0x44, 0x3C, 0x48 };

    // UI \\

    while (true)
    {
        if (!FindWindowA(NULL, "Bla Bla"))
        {
            std::cout << "Dude simulator window closed, closing in 3 seconds...";
            Sleep(1000);
            system("CLS");
            std::cout << "Dude simulator window closed, closing in 2 seconds...";
            Sleep(1000);
            system("CLS");
            std::cout << "Dude simulator window closed, closing in 1 seconds...";
            Sleep(1000);
            system("CLS");

            return 0;
        }

        std::cout << "   __       __                        __     \n  / \     /  |                      /  |\n  $$  \ / $$ | __    __   _______  _$$ | _\n  $$$  \ / $$$ |/  |  /  | /       |/ $$ |\n  $$$$ / $$$$ | $$ | $$ |/$$$$$$$ / $$$$$$ /\n  $$ $$ $$ / $$ | $$ | $$ | $$      \   $$ | __\n  $$ | $$$ / $$ | $$ \__$$ | $$$$$$ | $$ |/  |\n  $$ | $ / $$ | $$    $$ |/     $$ / $$  $$ /\n  $$ / $$ / $$$$$$$ | $$$$$$$ / $$$$/\n               /  \__$$ |\n               $$    $$ /\n                $$$$$$ / \n\n";

        std::cout << " Dm https://discordapp.com/users/717369125965398027 if you find any bugs or it doesnt work\n\n";

        int cash;
        std::cout << " [+] Enter Cash Amount: ";
        std::cin >> cash;

        if (!FindWindowA(NULL, "Bla Bla"))
        {
            std::cout << "\nDude simulator window closed, closing in 3 seconds...";
            Sleep(1000);
            system("CLS");
            std::cout << "Dude simulator window closed, closing in 2 seconds...";
            Sleep(1000);
            system("CLS");
            std::cout << "Dude simulator window closed, closing in 1 seconds...";
            Sleep(1000);
            system("CLS");

            return 0;
        }

        try
        {
            WriteProcessMemory(process, (LPVOID*)GetPointerAddress(window, client, cashAddr, cashOffsets), &cash, 4, 0);
            system("CLS");
        }
        catch(std::exception &err)
        {
            system("CLS");
        }
    }

    return 0;
}