#include "hookdll.h"
#include <iostream>

HHOOK g_hHook = NULL;
int g_lastKeyInfo = 0; // 32-bit integer to store vkCode and isKeyDown

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        int vkCode = ((KBDLLHOOKSTRUCT *)lParam)->vkCode;
        int isKeyDown = (wParam == WM_KEYDOWN) ? 1 : 0;

        // Pack vkCode and isKeyDown into a single int
        g_lastKeyInfo = (vkCode & 0xFFFF) | ((isKeyDown & 0x1) << 16);

        if (isKeyDown)
        {
            std::cout << "Key pressed: " << vkCode << std::endl;
        }
        else
        {
            std::cout << "Key released: " << vkCode << std::endl;
        }
    }
    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void InstallHook()
{
    g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);
    if (g_hHook == NULL)
    {
        std::cerr << "Failed to install hook!" << std::endl;
    }
}

void UninstallHook()
{
    if (g_hHook != NULL)
    {
        UnhookWindowsHookEx(g_hHook);
        g_hHook = NULL;
    }
}

int GetLastKeyInfo()
{
    return g_lastKeyInfo;
}