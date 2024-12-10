#include <windows.h>
#include <iostream>
#include <functional>

typedef LRESULT (CALLBACK *KeyboardProcType)(int nCode, WPARAM wParam, LPARAM lParam);
typedef void (*InstallHookType)();
typedef void (*UninstallHookType)();
typedef int (*GetLastKeyInfoType)(); // Change return type to int

int main()
{
    HMODULE hModule = LoadLibrary(TEXT("hookdll.dll"));
    if (!hModule)
    {
        std::cerr << "Failed to load DLL!" << std::endl;
        return 1;
    }

    InstallHookType InstallHook = (InstallHookType)GetProcAddress(hModule, "InstallHook");
    UninstallHookType UninstallHook = (UninstallHookType)GetProcAddress(hModule, "UninstallHook");
    GetLastKeyInfoType GetLastKeyInfo = (GetLastKeyInfoType)GetProcAddress(hModule, "GetLastKeyInfo");

    if (!InstallHook || !UninstallHook || !GetLastKeyInfo)
    {
        std::cerr << "Failed to get function addresses!" << std::endl;
        FreeLibrary(hModule);
        return 1;
    }

    InstallHook();

    while (true)
    {
        int keyInfo = GetLastKeyInfo();
        int vkCode = keyInfo & 0xFFFF;
        int isKeyDown = (keyInfo >> 16) & 0x1;

        if (vkCode != 0)
        {
            if (isKeyDown)
            {
                std::cout << "Main program received key press: " << vkCode << std::endl;
            }
            else
            {
                std::cout << "Main program received key release: " << vkCode << std::endl;
            }
        }
        Sleep(100); // Sleep for a short time to avoid high CPU usage
    }

    UninstallHook();
    FreeLibrary(hModule);

    return 0;
}