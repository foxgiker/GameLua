/*
 * Descp: 所有需要暴露给lua脚本的控制方法，都由这个类实现
 *
 *
 */

#ifndef GAMELUA_LUACONTROL_H
#define GAMELUA_LUACONTROL_H

#include <QDebug>
#include <Windows.h>
#include <QThread>

#include "KeyMouseMgr.h"
#include "CVHelper.h"

//虚拟键表
static QMap<QString,uint> VIRTUAL_KEY = {
        {"VK_LBUTTON",   0x01},
        {"VK_RBUTTON",   0x02},
        {"VK_CANCEL",    0x03},
        {"VK_MBUTTON",   0x04},   //鼠标中键
        {"VK_XBUTTON1",  0x05},
        {"VK_XBUTTON2",  0x06},
        {"VK_BACK",      0x08},
        {"VK_TAB",       0x09},
        {"VK_CLEAR",     0x0C},
        {"VK_RETURN",    0x0D},
        {"VK_SHIFT",     0x10},
        {"VK_CONTROL",   0x11},
        {"VK_MENU",      0x12},   // ALT
        {"VK_PAUSE",     0x13},
        {"VK_CAPITAL",   0x14},
        {"VK_KANA",      0x15},   //输入法kana模式
        {"VK_HANGUEL",   0x15},
        {"VK_HANGUL",    0x15},
        {"VK_IME_ON",    0x16},  //IME ON
        {"VK_JUNJA",     0x17},  // IME Junja mode
        {"VK_FINAL",     0x18},  //IME final mode
        {"VK_HANJA",     0x19},
        {"VK_KANJI",     0x19},
        {"VK_IME_OFF",   0x1A},
        {"VK_ESCAPE",    0x1B},
        {"VK_CONVERT",   0x1C},  // IME convert
        {"VK_NONCONVERT",0x1D},  // IME nonconvert
        {"VK_ACCEPT",    0x1E},  // IME accept
        {"VK_MODECHANGE",0x1F},  // IME mode change request
        {"VK_SPACE",     0x20},
        {"VK_PRIOR",     0x21},  // PAGE UP key
        {"VK_NEXT",      0x22},  // PAGE DOWN key
        {"VK_END",       0x23},  // END key
        {"VK_HOME",      0x24},  // HOME key
        {"VK_LEFT",      0x25},  // LEFT ARROW key
        {"VK_UP",        0x26},   //UP ARROW key
        {"VK_RIGHT",     0x27},
        {"VK_DOWN",      0x28},
        {"VK_SELECT",    0x29},
        {"VK_SNAPSHOT",  0x2c},
        {"VK_DELETE",    0x2e},

        {"VK_0",         0x30},
        {"VK_1",         0x31},
        {"VK_2",         0x32},
        {"VK_3",         0x33},
        {"VK_4",         0x34},
        {"VK_5",         0x35},
        {"VK_6",         0x36},
        {"VK_7",         0x37},
        {"VK_8",         0x38},
        {"VK_9",         0x39},

        {"VK_A",         0x41},
        {"VK_B",         0x42},
        {"VK_C",         0x43},
        {"VK_D",         0x44},
        {"VK_E",         0x45},
        {"VK_F",         0x46},
        {"VK_G",         0x47},
        {"VK_H",         0x48},
        {"VK_I",         0x49},
        {"VK_J",         0x4A},
        {"VK_K",         0x4B},
        {"VK_L",         0x4C},
        {"VK_M",         0x4D},
        {"VK_N",         0x4E},
        {"VK_O",         0x4F},
        {"VK_P",         0x50},
        {"VK_Q",         0x51},
        {"VK_R",         0x52},
        {"VK_S",         0x53},
        {"VK_T",         0x54},
        {"VK_U",         0x55},
        {"VK_V",         0x56},
        {"VK_W",         0x57},
        {"VK_X",         0x58},
        {"VK_Y",         0x59},
        {"VK_Z",         0x5A},

        {"VK_NUMPAD0",   0x60},   // 数字键盘区域
        {"VK_NUMPAD1",   0x61},
        {"VK_NUMPAD2",   0x62},
        {"VK_NUMPAD3",   0x63},
        {"VK_NUMPAD4",   0x64},
        {"VK_NUMPAD5",   0x65},
        {"VK_NUMPAD6",   0x66},
        {"VK_NUMPAD7",   0x67},
        {"VK_NUMPAD8",   0x68},
        {"VK_NUMPAD9",   0x69},
        {"VK_MULTIPLY",  0x6a},
        {"VK_ADD",       0x6b},
        {"VK_SEPARATOR", 0x6c},
        {"VK_SUBTRACT",  0x6d},
        {"VK_DECIMAL",   0x6e},
        {"VK_DIVIDE",    0x6f},
                                            // F功能键区域
        {"VK_F1",        0x70},
        {"VK_F2",        0x71},
        {"VK_F3",        0x72},
        {"VK_F4",        0x73},
        {"VK_F5",        0x74},
        {"VK_F6",        0x75},
        {"VK_F7",        0x76},
        {"VK_F8",        0x77},
        {"VK_F9",        0x78},
        {"VK_F10",       0x79},
        {"VK_F11",       0x7A},
        {"VK_F12",       0x7B},
        {"VK_SCROLL",    0x91}
};



class LuaControl {

private:
    uint _hwnd;
public:
    explicit LuaControl(uint hwnd);
    ~LuaControl();
    void msleep(unsigned long  msecs);
    void sleep(unsigned long usecs);
    void keyDown(int key);
    void keyUp(int key);
    void keyLongPress(int vkey, long sec);
    void activeWindow();
    void mouseMoveToPoint(int,int);
    void mouseMoveToPointClick(int,int);
    POINT findPicPos(char const* szfile);   // 找到标定位置在窗口的位置， 需要试用clienttoscreen 转换位屏幕坐标
    POINT findPicPosScreen(char const* szfile);   // 找到标定位置在窗口的位置， 需要试用clienttoscreen 转换位屏幕坐标
};


#endif //GAMELUA_LUACONTROL_H
