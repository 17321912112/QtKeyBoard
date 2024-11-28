#include "inputmethodmgr.h"

#include <windows.h>

namespace KeyBoard
{
    
    void ClickKey(int vk_code)
    {
        INPUT ip; 
        ip.type = INPUT_KEYBOARD; 
        ip.ki.wVk = vk_code; 
        ip.ki.dwFlags = 0; 
        SendInput(1, &ip, sizeof(INPUT)); 
        ip.ki.dwFlags = KEYEVENTF_KEYUP; 
        SendInput(1, &ip, sizeof(INPUT)); 
    }

    void PressKey(int vk_code)
    {
        INPUT ip; 
        ip.type = INPUT_KEYBOARD; 
        ip.ki.wVk = vk_code; 
        ip.ki.dwFlags = 0; 
        SendInput(1, &ip, sizeof(INPUT)); 
    }

    void ReleaseKey(int vk_code)
    {
        INPUT ip; 
        ip.type = INPUT_KEYBOARD; 
        ip.ki.wVk = vk_code; 
        ip.ki.dwFlags = KEYEVENTF_KEYUP; 
        SendInput(1, &ip, sizeof(INPUT)); 
    }

    class InputMethodMgr::CPrivate
    {
    public:
        CPrivate(InputMethodMgr* mParent)
            :mParent(mParent)
        {

        }

        InputMethodMgr *mParent;
    };


    InputMethodMgr::InputMethodMgr(QObject *parent)
        : QObject(parent)
        , md(new CPrivate(this))
    {

    }
}
