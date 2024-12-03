#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QObject>
#include <QMap>
#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <windows.h>

namespace KeyBoard
{
    enum Language  // 键盘语言
    {
        Language_English, 
        Language_Chinese, 
        Language_Unknown, 
    };

    enum LayoutMode // 键盘布局语言状态(Shift切换)
    {
        Mode_English,
        Mode_Chinese,
        Mode_Unknown,
    };

    enum KeyType
    {
        Key_0 = 0x30, Key_1, Key_2, Key_3, Key_4, Key_5, Key_6, Key_7, Key_8, Key_9, // 数字

        Key_A = 0x41,
        Key_B, Key_C, Key_D, Key_E, Key_F, Key_G, Key_H, Key_I, Key_J,               // 字母
        Key_K, Key_L, Key_M, Key_N, Key_O, Key_P, Key_Q, Key_R, Key_S, Key_T,
        Key_U, Key_V, Key_W, Key_X, Key_Y, Key_Z,

        Key_LBUTTON = 0x01,
        Key_RBUTTON = 0x02,
        Key_CANCEL = 0x03,
        Key_MBUTTON = 0x04,
        Key_XBUTTON1 = 0x05,
        Key_XBUTTON2 = 0x06,
        Key_BACK = 0x08,
        Key_TAB = 0x09,
        Key_CLEAR = 0x0C,
        Key_RETURN = 0x0D,
        Key_SHIFT = 0x10,
        Key_CONTROL = 0x11,
        Key_MENU = 0x12,
        Key_PAUSE = 0x13,
        Key_CAPITAL = 0x14,
        Key_KANA = 0x15,
        Key_HANGEUL = 0x15,
        Key_HANGUL = 0x15,
        Key_JUNJA = 0x17,
        Key_FINAL = 0x18,
        Key_HANJA = 0x19,
        Key_KANJI = 0x19,
        Key_ESCAPE = 0x1B,
        Key_CONVERT = 0x1C,
        Key_NONCONVERT = 0x1D,
        Key_ACCEPT = 0x1E,
        Key_MODECHANGE = 0x1F,
        Key_SPACE = 0x20,
        Key_PRIOR = 0x21,
        Key_NEXT = 0x22,
        Key_END = 0x23,
        Key_HOME = 0x24,
        Key_LEFT = 0x25,
        Key_UP = 0x26,
        Key_RIGHT = 0x27,
        Key_DOWN = 0x28,
        Key_SELECT = 0x29,
        Key_PRINT = 0x2A,
        Key_EXECUTE = 0x2B,
        Key_SNAPSHOT = 0x2C,
        Key_INSERT = 0x2D,
        Key_DELETE = 0x2E,
        Key_HELP = 0x2F,

        Key_LWIN = 0x5B,
        Key_RWIN = 0x5C,
        Key_APPS = 0x5D,
        Key_SLEEP = 0x5F,
        Key_NUMPAD0 = 0x60,
        Key_NUMPAD1 = 0x61,
        Key_NUMPAD2 = 0x62,
        Key_NUMPAD3 = 0x63,
        Key_NUMPAD4 = 0x64,
        Key_NUMPAD5 = 0x65,
        Key_NUMPAD6 = 0x66,
        Key_NUMPAD7 = 0x67,
        Key_NUMPAD8 = 0x68,
        Key_NUMPAD9 = 0x69,
        Key_MULTIPLY = 0x6A,
        Key_ADD = 0x6B,
        Key_SEPARATOR = 0x6C,
        Key_SUBTRACT = 0x6D,
        Key_DECIMAL = 0x6E,
        Key_DIVIDE = 0x6F,
        Key_F1 = 0x70,
        Key_F2 = 0x71,
        Key_F3 = 0x72,
        Key_F4 = 0x73,
        Key_F5 = 0x74,
        Key_F6 = 0x75,
        Key_F7 = 0x76,
        Key_F8 = 0x77,
        Key_F9 = 0x78,
        Key_F10 = 0x79,
        Key_F11 = 0x7A,
        Key_F12 = 0x7B,
        Key_F13 = 0x7C,
        Key_F14 = 0x7D,
        Key_F15 = 0x7E,
        Key_F16 = 0x7F,
        Key_F17 = 0x80,
        Key_F18 = 0x81,
        Key_F19 = 0x82,
        Key_F20 = 0x83,
        Key_F21 = 0x84,
        Key_F22 = 0x85,
        Key_F23 = 0x86,
        Key_F24 = 0x87,
        Key_NUMLOCK = 0x90,
        Key_SCROLL = 0x91,
        Key_OEM_NEC_EQUAL = 0x92,
        Key_OEM_FJ_JISHO = 0x92,
        Key_OEM_FJ_MASSHOU = 0x93,
        Key_OEM_FJ_TOUROKU = 0x94,
        Key_OEM_FJ_LOYA = 0x95,
        Key_OEM_FJ_ROYA = 0x96,
        Key_LSHIFT = 0xA0,
        Key_RSHIFT = 0xA1,
        Key_LCONTROL = 0xA2,
        Key_RCONTROL = 0xA3,
        Key_LMENU = 0xA4,
        Key_RMENU = 0xA5,
        Key_BROWSER_BACK = 0xA6,
        Key_BROWSER_FORWARD = 0xA7,
        Key_BROWSER_REFRESH = 0xA8,
        Key_BROWSER_STOP = 0xA9,
        Key_BROWSER_SEARCH = 0xAA,
        Key_BROWSER_FAVORITES = 0xAB,
        Key_BROWSER_HOME = 0xAC,
        Key_VOLUME_MUTE = 0xAD,
        Key_VOLUME_DOWN = 0xAE,
        Key_VOLUME_UP = 0xAF,
        Key_MEDIA_NEXT_TRACK = 0xB0,
        Key_MEDIA_PREV_TRACK = 0xB1,
        Key_MEDIA_STOP = 0xB2,
        Key_MEDIA_PLAY_PAUSE = 0xB3,
        Key_LAUNCH_MAIL = 0xB4,
        Key_LAUNCH_MEDIA_SELECT = 0xB5,
        Key_LAUNCH_APP1 = 0xB6,
        Key_LAUNCH_APP2 = 0xB7,
        Key_OEM_1 = 0xBA,
        Key_OEM_PLUS = 0xBB,
        Key_OEM_COMMA = 0xBC,
        Key_OEM_MINUS = 0xBD,
        Key_OEM_PERIOD = 0xBE,
        Key_OEM_2 = 0xBF,
        Key_OEM_3 = 0xC0,
        Key_OEM_4 = 0xDB,
        Key_OEM_5 = 0xDC,
        Key_OEM_6 = 0xDD,
        Key_OEM_7 = 0xDE,
        Key_OEM_8 = 0xDF,
        Key_OEM_AX = 0xE1,
        Key_OEM_102 = 0xE2,
        Key_ICO_HELP = 0xE3,
        Key_ICO_00 = 0xE4,
        Key_PROCESSKEY = 0xE5,
        Key_ICO_CLEAR = 0xE6,
        Key_PACKET = 0xE7,
        Key_OEM_RESET = 0xE9,
        Key_OEM_JUMP = 0xEA,
        Key_OEM_PA1 = 0xEB,
        Key_OEM_PA2 = 0xEC,
        Key_OEM_PA3 = 0xED,
        Key_OEM_WSCTRL = 0xEE,
        Key_OEM_CUSEL = 0xEF,
        Key_OEM_ATTN = 0xF0,
        Key_OEM_FINISH = 0xF1,
        Key_OEM_COPY = 0xF2,
        Key_OEM_AUTO = 0xF3,
        Key_OEM_ENLW = 0xF4,
        Key_OEM_BACKTAB = 0xF5,
        Key_ATTN = 0xF6,
        Key_CRSEL = 0xF7,
        Key_EXSEL = 0xF8,
        Key_EREOF = 0xF9,
        Key_PLAY = 0xFA,
        Key_ZOOM = 0xFB,
        Key_NONAME = 0xFC,
        Key_PA1 = 0xFD,
        Key_OEM_CLEAR = 0xFE,

        Key_Unknown = -1
    };

    // const QMap<QChar, QChar> ShiftMap = {
    //     {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'},
    //     {'7', '&'}, {'8', '*'}, {'9', '('}, {'0', ')'}, {'\'', '"'}, {'-', '_'},
    //     {'=', '+'}, {'[', '{'}, {']', '}'}, {'\\', '|'}, {';', ':'}, {'\'', '"'},
    //     {',', '<'}, {'.', '>'}, {'/', '?'}, {'`', '~'}
    // };

    // const QMap<QChar, QChar> ShiftMapRevert = {
    //     {'!', '1'}, {'@', '2'}, {'#', '3'}, {'$', '4'}, {'%', '5'}, {'^', '6'},
    //     {'&', '7'}, {'*', '8'}, {'(', '9'}, {')', '0'}, {'"', '\''}, {'_', '-'},
    //     {'+', '='}, {'{', '['}, {'}', ']'}, {'|', '\\'}, {':', ';'}, {'"', '\''},
    //     {'<', ','}, {'>', '.'}, {'?', '/'}, {'~', '`'}
    // };

    const QMap<QString, QString> EnglishCharMap = {
        {"1", "!"}, {"2", "@"}, {"3", "#"}, {"4", "$"}, {"5", "%"}, {"6", "^"},
        {"7", "&"}, {"8", "*"}, {"9", "("}, {"0", ")"}, {"\'", "\""}, {"-", "_"},
        {"= ", "+"}, {"[", "{"}, {"]", "}"}, {"\\", "|"}, {";", ":"}, {"\'", "\""},
        {",", "<"}, {".", ">"}, {"/", "?"}
    };

    const QMap<QString, QString> EnglishCharMapRevert = {
        {"!", "1"}, {"@", "2"}, {"#", "3"}, {"$", "4"}, {"%", "5"}, {"^", "6"},
        {"&", "7"}, {"*", "8"}, {"(", "9"}, {")", "0"}, {"_", "-"},
        {"+", "="}, {"{", "["}, {"}", "]"}, {"|", "\\"}, {":", ";"}, {"\"", "\'"},
        {"<", ","}, {">", "."}, {"?", "/"}
    };

   const QMap<QString, QString> ChineseCharMap = {
        {"1", "！"}, {"2", "@"}, {"3", "#"}, {"4", "￥"}, {"5", "%"}, {"6", "……"},
        {"7", "&"}, {"8", "*"}, {"9", "（"}, {"0", "）"}, {"-", "——"}, {"=", "+"},
        {"[", "【"}, {"]", "】"}, {"\\", "、"}, {";", "："}, {"'", "‘"}, {",", "《"},
        {".", "》"}, {"/", "？"}
   };

   const QMap<QString, QString> ChineseCharMapRevert = {
        {"！", "1"}, {"@", "2"}, {"#", "3"}, {"￥", "4"}, {"%", "5"}, {"……", "6"},
        {"&", "7"}, {"*", "8"}, {"（", "9"}, {"）", "0"}, {"——", "-"}, {"+", "="},
        {"【", "["}, {"】", "]"}, {"、", "\\"}, {":", ";"}, {"‘", "'"}, {"《", ","},
        {"》", "."}, {"？", "/"}
   };

   const QMap<QString, QString> EnglishMapChinese = {
        {"!", "！"}, {"@", "@" }, {"#", "#"}, {"$", "￥"}, {"%", "%"}, {"^", "……"},
        {"&", "&"}, {"*", "*"}, {"(", "（"}, {")", "）"}, {"_", "——"}, {"+", "+"},
        {"{", "【"}, {"}", "】"}, {"|", "、"}, {":", "："}, {"\"", "‘"}, {"<", "《"},
        {">", "》"}, {"?", "？"}
   };

   const QMap<QString, QString> ChineseMapEnglish = {
        {"！", "!"}, {"@", "@" }, {"#", "#"}, {"￥", "$"}, {"%", "%"}, {"……", "^"},
        {"&", "&"}, {"*", "*"}, {"（", "("}, {")", ")"}, {"——", "_"}, {"+", "+"},
        {"【", "{"}, {"】", "}"}, {"、", "|"}, {"：", ":"}, {"‘", "\""}, {"《", "<"},
        {"》", ">"}, {"？", "/"}
   };

    void ClickKey(int vk_code);

    void PressKey(int vk_code);

    void ReleaseKey(int vk_code);

    // 指定按键码是否处于激活状态，检查系统大小写状态
    bool GetKeyOpenState(int vk_code);

    // 返回是否是字符按键 如 '1 !' '2 @' '; :'
    bool IsCharKey(int vk_code); 

    // 返回是否是字母按键 如 'a A' 'b B' 'c C'
    bool IsLetterKey(int vk_code); 

    // 以下为windows输入法api调用
    void SetConversionMode(DWORD dwConversionMode);

    // 获取当前窗口的输入法上下文
    HIMC GetActiveIMMContext();

    // 获取输入法当前模式
    bool GetConversionMode(DWORD &dwConversionMode);

    // 获取输入法当前语言状态
    LayoutMode GetLanguageState(); 

    // 获取当前输入法是否打开
    bool IsIMEOpen();

    // 设置当前输入法的打开状态
    bool SetIMEOpenStatus(bool isOpen);

    // 微软拼音中英切换
    void MicroSoftSwitch();

    // class InputMethodMgr : public QObject
    // {
    //     Q_OBJECT
    // public:
    //     explicit InputMethodMgr(QObject *parent = nullptr);

    // signals:

    // private:
    //     class CPrivate;
    //     CPrivate *const md;

    // };
}

#endif // INPUTMETHOD_H
