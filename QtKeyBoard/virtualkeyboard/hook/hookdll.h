#ifndef HOOKDLL_H
#define HOOKDLL_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
__declspec(dllexport) void InstallHook();
__declspec(dllexport) void UninstallHook();
__declspec(dllexport) int GetLastKeyInfo();

#ifdef __cplusplus
}
#endif

#endif // HOOKDLL_H