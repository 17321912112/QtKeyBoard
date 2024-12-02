#include "inputmethod.h"

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


    // 获取当前窗口的输入法上下文
    HIMC GetActiveIMMContext() {
        HWND hwnd = GetForegroundWindow(); // 获取当前激活的窗口句柄
        // qDebug() << "hwnd:" << hwnd;
        if (!hwnd) return NULL;
        return ImmGetContext(hwnd);
    }

    // 获取当前输入法的中英文模式
    /*
        dwConversionMode
        dwConversionMode 表示输入法的转换模式，即输入法当前的输入状态。它是一个位标志，可以组合使用多个标志来表示不同的输入模式。常见的标志包括：
        IME_CMODE_ALPHANUMERIC (0x0000)：字母数字模式（英文模式）。
        IME_CMODE_NATIVE (0x0001)：本地模式（中文模式）。
        IME_CMODE_KATAKANA (0x0002)：片假名模式。
        IME_CMODE_FULLSHAPE (0x0008)：全角模式。
        IME_CMODE_ROMAN (0x0010)：罗马字模式。
        IME_CMODE_CHARCODE (0x0020)：字符代码模式。
        IME_CMODE_HANJACONVERT (0x0040)：汉字转换模式。
        IME_CMODE_SOFTKBD (0x0080)：软键盘模式。
        IME_CMODE_NOCONVERSION (0x0100)：不转换模式。
        IME_CMODE_EUDC (0x0200)：扩展用户定义字符模式。
        IME_CMODE_SYMBOL (0x0400)：符号模式。
        IME_CMODE_FIXEDWIDTH (0x1000)：固定宽度模式。
    */

    bool GetConversionMode(DWORD &dwConversionMode) 
    {
        // 获取当前激活的输入法上下文句柄
        HIMC hIMC = GetActiveIMMContext();
        if (!hIMC) return false;
        // 定义一个变量用于存储句子模式
        DWORD dwSentenceMode;
        // 获取输入法的转换状态和句子模式
        BOOL result = ImmGetConversionStatus(hIMC, &dwConversionMode, &dwSentenceMode);
        // qDebug() << "conversion mode: " << dwConversionMode << " sentence mode: " << dwSentenceMode << "result: " << result;
        // 释放输入法上下文句柄
        ImmReleaseContext(GetForegroundWindow(), hIMC);
        // 返回获取状态的结果
        return result == TRUE;
    }

    void SetConversionMode(DWORD dwConversionMode)
    {
        HIMC hIMC = GetActiveIMMContext();
        if (!hIMC) return;

        DWORD dwSentenceMode, con;
        ImmGetConversionStatus(hIMC, &con, &dwSentenceMode);
        // qDebug() << "set conversion mode:" << dwConversionMode;
        BOOL result = ImmSetConversionStatus(hIMC, dwConversionMode, dwSentenceMode);
        qDebug() << "SetConversionMode result:" << result;
        ImmReleaseContext(GetForegroundWindow(), hIMC);
    }

    LanguageType GetLanguageState()
    {
        LanguageType languageType = LanguageType::Language_Unknown;

        // 判断当前输入法的中英文状态
        DWORD conversionMode;
        if (GetConversionMode(conversionMode)) 
        {
            if (conversionMode)
            {
                // qDebug() << "当前处于中文模式";
                languageType = Language_Chinese; 
            }
            else
            {
                // qDebug() << "当前处于英文模式";
                languageType = Language_English;
            }
        } else {
            qDebug() << "无法获取转换模式";
        }
        
        return languageType;
    }

     // 获取当前输入法是否打开
    bool IsIMEOpen()
    {
        HIMC hIMC = GetActiveIMMContext();
        if (!hIMC) return false;

        BOOL isOpen = ImmGetOpenStatus(hIMC);
        qDebug() << "IME Open Status: " << (isOpen ? "Open" : "Closed");

        ImmReleaseContext(GetForegroundWindow(), hIMC);
        return isOpen == TRUE;
    }


     // 设置当前输入法的打开状态
    bool SetIMEOpenStatus(bool isOpen)
    {
        HIMC hIMC = GetActiveIMMContext();
        if (!hIMC) return false;

        BOOL result = ImmSetOpenStatus(hIMC, isOpen);
        qDebug() << "Set IME Open Status to " << (isOpen ? "Open" : "Closed") << " Result: " << (result ? "Success" : "Failed");

        ImmReleaseContext(GetForegroundWindow(), hIMC);
        return result == TRUE;
    }

    void MicroSoftSwitch()
    {
        if (!IsIMEOpen())
        {
            if (!SetIMEOpenStatus(true))
            {
                qDebug() << "open ime failed";
            }
        }
        auto l = KeyBoard::GetLanguageState();
        // qDebug() << "before: Switch"; 
        DWORD dwConversionMode = 0x0401;
        switch (l)
        {
            case KeyBoard::Language_Chinese:
            {
                dwConversionMode = 0x0000;
                KeyBoard::SetConversionMode(dwConversionMode);
                
            }break;
            case KeyBoard::Language_English:
            {
                dwConversionMode = 0x0401;
                KeyBoard::SetConversionMode(dwConversionMode);
               
            }break;
            default:
                break;
        }
        // qDebug() << "after: Switch"; 
        KeyBoard::GetLanguageState();
    }

//    class InputMethodMgr::CPrivate
//    {
//    public:
//        CPrivate(InputMethodMgr* mParent)
//            :mParent(mParent)
//        {

//        }

//        InputMethodMgr *mParent;
//    };


//    InputMethodMgr::InputMethodMgr(QObject *parent)
//        : QObject(parent)
//        , md(new CPrivate(this))
//    {

//    }
}
