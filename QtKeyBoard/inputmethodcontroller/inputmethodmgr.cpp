#include "inputmethodmgr.h"

namespace KeyBoard
{
    
    void ClickKey(int vk_code)
    {
        if (vk_code != Key_Unknown)
        {
            INPUT ip; 
            ip.type = INPUT_KEYBOARD; 
            ip.ki.wVk = vk_code; 
            ip.ki.dwFlags = 0; 
            SendInput(1, &ip, sizeof(INPUT)); 
            ip.ki.dwFlags = KEYEVENTF_KEYUP; 
            SendInput(1, &ip, sizeof(INPUT)); 
        }
    }

    void PressKey(int vk_code)
    {
        if (vk_code != Key_Unknown)
        {
            INPUT ip; 
            ip.type = INPUT_KEYBOARD; 
            ip.ki.wVk = vk_code; 
            ip.ki.dwFlags = 0; 
            SendInput(1, &ip, sizeof(INPUT)); 
        }
    }

    void ReleaseKey(int vk_code)
    {
        if (vk_code != Key_Unknown)
        {
            INPUT ip; 
            ip.type = INPUT_KEYBOARD; 
            ip.ki.wVk = vk_code; 
            ip.ki.dwFlags = KEYEVENTF_KEYUP; 
            SendInput(1, &ip, sizeof(INPUT)); 
        }
    }

    bool GetKeyOpenState(int vk_code)
    {
        return GetKeyState(vk_code) & 0x0001 ? true : false; // 按键状态
    }

    bool IsCharKey(int vk_code)
    {
        bool res = false;
        if (vk_code >= Key_0 && vk_code <= Key_9)
        {
            res = true;
        }
        else if (vk_code >= Key_MULTIPLY && vk_code <= Key_DIVIDE)
        {
            res = true;
        }
        else if (vk_code >= Key_OEM_1 && vk_code <= Key_OEM_8)
        {
            res = true;
        }
        return res;
    }

    bool IsLetterKey(int vk_code)
    {
        return vk_code >= Key_A && vk_code <= Key_Z;
    }


    // 定义一个函数来获取当前窗口的输入法上下文
    HIMC GetActiveIMMContext() {
        HWND hwnd = GetForegroundWindow(); // 获取当前激活的窗口句柄
        qDebug() << "hwnd:" << hwnd;
        if (!hwnd) return NULL;
        return ImmGetContext(hwnd);
    }

    // 获取当前输入法的中英文模式
    bool GetConversionMode(DWORD &dwConversionMode) {
        HIMC hIMC = GetActiveIMMContext();
        if (!hIMC) return false;

        DWORD dwSentenceMode;
        BOOL result = ImmGetConversionStatus(hIMC, &dwConversionMode, &dwSentenceMode);
        ImmReleaseContext(GetForegroundWindow(), hIMC);
        return result == true;
    }

    LanguageType GetLanguageState()
    {
        LanguageType languageType = LanguageType::Language_Unknown;
        // 获取当前键盘布局

        // 判断当前输入法的中英文状态
        DWORD conversionMode;
        if (GetConversionMode(conversionMode)) {
            if (conversionMode & IME_CMODE_ALPHANUMERIC) {
                qDebug() << "当前处于英文模式";
            } else {
                qDebug() << "当前处于中文模式";
            }
        } else {
            qDebug() << "无法获取转换模式";
        }
        
        return languageType;
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
