//
// Created by c0518535x on 2023/7/28.
//

#include <QDebug>
#include "KeyMouseMgr.h"



// 初始化winio
gm::KeyMouseMgr::KeyMouseMgr(){

}

// 清理
gm::KeyMouseMgr::~KeyMouseMgr() {

}

void gm::KeyMouseMgr::keyLongPress(int key, long sec) {
    // 模拟长安一个键一段时间
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.dwFlags = 0; // Key press

    // Send the key press event
    SendInput(1, &input, sizeof(INPUT));
    // Pause for the specified duration
    Sleep(sec*1000);
    // Release the key
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    // Send the key release event
    SendInput(1, &input, sizeof(INPUT));
}


void gm::KeyMouseMgr::keyDown(int key) {
    // keybd_event 方式
//    int scode = MapVirtualKey(key,0);
//    keybd_event(key,scode,0,0);
// 使用 SendInput实现

    SHORT f = GetAsyncKeyState(key);
    SHORT g = GetKeyState(key);
    qDebug() << "async key state: " << f << "key state: " << g;

    INPUT inputs[1]  = {};
    ZeroMemory(inputs,sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = key;
    inputs[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    inputs[0].ki.wScan = MapVirtualKey(key,0);
    UINT uSent = SendInput(ARRAYSIZE(inputs),inputs,sizeof(INPUT));
    if(uSent != ARRAYSIZE(inputs)){
        qDebug() <<"Send Failed： 只发送了" << uSent <<"," << HRESULT_FROM_WIN32(GetLastError());
    }else{
        // 同步键盘状态
        BYTE keyboardState[256] = {0};
        keyboardState[key] = 0x8000;
        SetKeyboardState(keyboardState);
    }

// 使用sendmessage 试试看
//    WORD loword = 1;
//    WORD scancode = 0;
//    WORD extendedkey = 0x100;
//    WORD keyup = 0| 1| 1;
//    WORD keydown = 0 | 1 |0;
//    WORD hiword = scancode |extendedkey |keydown;
//    SendMessage(GetForegroundWindow(),WM_KEYDOWN,key, MAKELPARAM(loword,hiword));

}

void gm::KeyMouseMgr::keyUp(int key) {
    //keybd_event(key,MapVirtualKey(key,0),KEYEVENTF_KEYUP,0);

    SHORT f = GetAsyncKeyState(key);
    SHORT g = GetKeyState(key);
    qDebug() << "async key state: " << f << "key state: " << g;

    INPUT inputs[1]  = {};
    ZeroMemory(inputs,sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
    inputs[0].ki.wVk = key;
    inputs[0].ki.wScan = MapVirtualKey(key,0);
    UINT uSent = SendInput(ARRAYSIZE(inputs),inputs,sizeof(INPUT));
    if(uSent != ARRAYSIZE(inputs)){
        qDebug() <<"Send Failed： 只发送了" << HRESULT_FROM_WIN32(GetLastError());
    }else{
        BYTE keyboardState[256] = {0};
        keyboardState[key] = 0;
        SetKeyboardState(keyboardState);
    }

}


/**
 *
 * @param x
 * @param y
 */
void gm::KeyMouseMgr::MouseMoveToPoint(int x, int y) {
    SetCursorPos(x, y);
}


void gm::KeyMouseMgr::MouseMoveToPointClick(int x, int y) {
    SetCursorPos(x, y);
    // 鼠标左键点击
    mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_ABSOLUTE, 0, 0, 0, NULL);
    // 鼠标左键抬起
    mouse_event(MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE, 0, 0, 0, NULL);
}


