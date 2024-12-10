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

    void HandleBackspace()
    {
        if (IsIMEOpen()) {
            DWORD conversionMode;
            if (GetConversionMode(conversionMode)) {
                // 如果 IME 处于组合字符模式
                if (conversionMode & 0x400) 
                {
                    // 调用 IME 的方法来处理退格键
                    HWND hwnd = GetForegroundWindow();
                    if (hwnd) 
                    {
                        SendMessage(hwnd, WM_IME_NOTIFY, IMN_SETCONVERSIONMODE, 0);
                        SendMessage(hwnd, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, 0);
                    }
                } 
                else 
                {
                    // 如果 IME 不处于组合字符模式，按普通退格键处理
                    PressKey(VK_BACK);
                    ReleaseKey(VK_BACK);
                }
            }
        } 
        else 
        {
            // 如果 IME 关闭，按普通退格键处理
            ClickKey(VK_BACK);
        }
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
    HIMC GetActiveIMMContext() 
    {
        HWND hwnd = GetForegroundWindow(); // 获取当前激活的窗口句柄
        // qDebug() << "hwnd:" << hwnd;
        if (!hwnd) return NULL;
        return ImmGetContext(hwnd);
    }

    // 获取当前输入法的中英文模式
    /*
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
        if (!result)
        {
            qDebug() << "SetConversionMode result failed";
        }
        ImmReleaseContext(GetForegroundWindow(), hIMC);
    }

    LayoutMode GetLanguage()
    {
        return CURRENT_LANGUAGE;
    }

    LayoutMode GetLanguageState()
    {
        LayoutMode languageType = LayoutMode::Mode_Unknown;

        // 判断微软输入法的中英文状态
        DWORD conversionMode;
        if (GetConversionMode(conversionMode)) 
        {
            if (conversionMode)
            {
                // qDebug() << "当前处于中文模式";
                languageType = Mode_Chinese; 
            }
            else
            {
                // qDebug() << "当前处于英文模式";
                languageType = Mode_English;
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
        if (!isOpen)
        {
//            qDebug() << "IME Open Status: " << (isOpen ? "Open" : "Closed");
        }

        ImmReleaseContext(GetForegroundWindow(), hIMC);
        return isOpen == TRUE;
    }


     // 设置当前输入法的打开状态
    bool SetIMEOpenStatus(bool isOpen)
    {
        HIMC hIMC = GetActiveIMMContext();
        if (!hIMC) return false;

        BOOL result = ImmSetOpenStatus(hIMC, isOpen);
        if (!result)
        {
//            qDebug() << "Set IME Open Status to " << (isOpen ? "Open" : "Closed") << " Result: " << (result ? "Success" : "Failed");
        }

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
        // 中英文切换
        DWORD dwConversionMode = 0x0401;
        switch (l)
        {
            case KeyBoard::Mode_Chinese:
            {
                dwConversionMode = 0x0000;
                KeyBoard::SetConversionMode(dwConversionMode);
                
            }break;
            case KeyBoard::Mode_English:
            {
                dwConversionMode = 0x0401;
                KeyBoard::SetConversionMode(dwConversionMode);
            }break;
            default:
                break;
        }
        KeyBoard::GetLanguageState();
    }

    // 搜狗输入法中文
    void SetSogouChinese()
    {
        SetIMEOpenStatus(true);
        SetConversionMode(0x0401);
        CURRENT_LANGUAGE = LayoutMode::Mode_Chinese;
    }

    // 搜狗输入法英文
    void SetSogouEnglish()
    {
        SetIMEOpenStatus(false);
        SetConversionMode(0x0000);
        CURRENT_LANGUAGE = LayoutMode::Mode_English;
    }

    void SogouSwitch()
    {
        if (CURRENT_LANGUAGE == LayoutMode::Mode_Chinese)
        {
            SetSogouEnglish();
        }
        else if (CURRENT_LANGUAGE == LayoutMode::Mode_English)
        {
            SetSogouChinese();
        }
    }

    void getPreeditCandidates()
    {
        HWND hwnd = GetForegroundWindow(); 
        HIMC hIMC = ImmGetContext(hwnd);
        if (hIMC == NULL) {
//            qDebug() << "Failed to get input context";
            return;
        }

        unsigned long dwListCount = 0;
        if (ImmGetCandidateListCountA(hIMC, &dwListCount) == 0) {
            ImmReleaseContext(hwnd, hIMC);
//            qDebug() << "Failed to get candidate list count";
            return;
        }

        for (UINT i = 0; i < dwListCount; ++i) {
            CANDIDATELIST list;
            if (ImmGetCandidateListA(hIMC, i, &list, sizeof(list)) == 0) {
//                qDebug() << "Failed to get candidate list" << i;
                continue;
            }

            for (UINT j = 0; j < list.dwCount; ++j) {
//                qDebug() << "Candidate" << j << ":" << QString::fromWCharArray((wchar_t*)list.dwOffset[j]);
            }
        }

        ImmReleaseContext(hwnd, hIMC);
    }

    
    QUuid GetCurrentIMEGuid()
    {
        HWND hwnd = GetForegroundWindow();
        HIMC hIMC = ImmGetContext(hwnd);
        if (!hIMC) 
        {
            qDebug() << "Failed to get input context";
            return QUuid();
        }

         // 获取当前线程的键盘布局
        HKL hkl = GetKeyboardLayout(0);
        qDebug() << "Current HKL:" << hkl;
        if (hkl == NULL) 
        {
            qDebug() << "Failed to get HKL";
            ImmReleaseContext(hwnd, hIMC);
            return QUuid();
        }

         // 获取输入法的描述信息
        WCHAR description[256];
        if (ImmGetDescription(hkl, description, 256) == 0)
        {
            DWORD error = GetLastError();
            qDebug() << "Failed to get IME description, Error code" << error;
            ImmReleaseContext(hwnd, hIMC);
            return QUuid();
        }
        

        WCHAR imeFileName[MAX_PATH];
        if (ImmGetIMEFileName(hkl, imeFileName, MAX_PATH) == 0) 
        {
            DWORD error = GetLastError();
            qDebug() << "Failed to get IME file name, Error code" << error;
            ImmReleaseContext(hwnd, hIMC);
            return QUuid();
        }

        QString imeFilePath = QString::fromWCharArray(imeFileName);
        qDebug() << "Current IME file path:" << imeFilePath ;

        // 获取输入法的 GUID
        CLSID clsid;
        if (CLSIDFromString((LPOLESTR)imeFilePath.utf16(), &clsid) != S_OK) 
        {
            qDebug() << "Failed to convert IME file path to CLSID";
            ImmReleaseContext(hwnd, hIMC);
            return QUuid();
        }

        ImmReleaseContext(hwnd, hIMC);
        return QUuid(clsid);
    }

    // 判断当前输入法是否是微软拼音输入法
    bool IsMicrosoftPinyinIME()
    {
        QUuid currentIMEGuid = GetCurrentIMEGuid();
        if (currentIMEGuid.isNull()) 
        {
            qDebug() << "Failed to get current IME GUID";
            return false;
        }

        qDebug() << "Current IME GUID:" << currentIMEGuid.toString();

        if (currentIMEGuid == MS_PINYIN_GUID) 
        {
            qDebug() << "Current IME is Microsoft Pinyin Input Method.";
            return true;
        } 
        else 
        {
            qDebug() << "Current IME is not Microsoft Pinyin Input Method.";
            return false;
        }
    }



    // void checkCurrentIME()
    // {
    //     HWND hwnd = GetForegroundWindow(); 
    //     HIMC hIMC = ImmGetContext(hwnd);
    //     if (hIMC == NULL) 
    //     {
    //         qDebug() << "Failed to get input context";
    //         return;
    //     }

    //     WCHAR imeFileName[MAX_PATH];
    //     if (ImmGetIMEFileName(hIMC, imeFileName, MAX_PATH) == 0) 
    //     {
    //         qDebug() << "Failed to get IME file name";
    //         ImmReleaseContext(hwnd, hIMC);
    //         return;
    //     }

    //     QString imeFilePath = QString::fromWCharArray(imeFileName);
    //     qDebug() << "Current IME file path:" << imeFilePath;

    //     // 判断是否为搜狗输入法
    //     if (imeFilePath.contains(L"sogou", Qt::CaseInsensitive)) 
    //     {
    //         qDebug() << "Current IME is Sogou Input Method.";
    //         // 获取搜狗输入法的码
    //         getSogouPinyinCode(hwnd);
    //     } 
    //     else 
    //     {
    //         qDebug() << "Current IME is not Sogou Input Method.";
    //     }

    //     ImmReleaseContext(hwnd, hIMC);
    // }

    int HookTest()
    {
        HMODULE hModule = LoadLibrary(TEXT("hookdll.dll"));
        if (!hModule)
        {
            qDebug() << "Failed to load DLL!";
            return 1;
        }

        InstallHookType InstallHook = (InstallHookType)GetProcAddress(hModule, "InstallHook");
        UninstallHookType UninstallHook = (UninstallHookType)GetProcAddress(hModule, "UninstallHook");
        GetLastKeyInfoType GetLastKeyInfo = (GetLastKeyInfoType)GetProcAddress(hModule, "GetLastKeyInfo");

        if (!InstallHook || !UninstallHook || !GetLastKeyInfo)
        {
            qDebug() << "Failed to get function addresses!";
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
                    qDebug() << "Main program received key press: " << vkCode;
                }
                else
                {
                    qDebug() << "Main program received key release: " << vkCode;
                }
            }
            Sleep(100); // Sleep for a short time to avoid high CPU usage
        }

        UninstallHook();
        FreeLibrary(hModule);

        return 0;
    }

    Language GetDefaultLanguage()
    {
        QString defaultLanguageName;
        Language defaultLanguage = Language::Language_Unknown;
        LCID lcid = GetUserDefaultLCID();
        char buffer[LOCALE_NAME_MAX_LENGTH];
        if (GetLocaleInfoA(lcid, LOCALE_SENGLANGUAGE, buffer, LOCALE_NAME_MAX_LENGTH)) 
        {
            qDebug() << "Language name:" << buffer;
            defaultLanguageName = buffer;
        }
        if (defaultLanguageName.contains("Chinese"))
        {
            defaultLanguage = Language::Language_Chinese;
        }
        else if (defaultLanguageName.contains("English"))
        {
            defaultLanguage = Language::Language_English;
        }
        return defaultLanguage;
    }

   class InputMethodMgr::CPrivate
   {
   public:
       CPrivate(InputMethodMgr* mParent)
           :mParent(mParent)
           , mode(LayoutMode::Mode_Unknown)
       {

       }

       InputMethodMgr *mParent;
       LayoutMode mode;
   };


   InputMethodMgr::InputMethodMgr(QObject *parent)
       : QObject(parent)
       , md(new CPrivate(this))
   {

   }
}
