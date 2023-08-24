//
// Created by c0518535x on 2023/8/14.
//

#include "LuaControl.h"



LuaControl::LuaControl(uint hwnd) {
    _hwnd = hwnd;
    qDebug() << "constructor hwnd" << hwnd;
}


LuaControl::~LuaControl() {

}


void LuaControl::activeWindow() {
    SetForegroundWindow((HWND)_hwnd);

//    HWND hTop = GetForegroundWindow();
//    if((unsigned int)hTop != _hwnd){
//        qDebug() << "设置窗体";
//        SetWindowPos((HWND)_hwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
//        SetForegroundWindow((HWND)_hwnd);
//        SetActiveWindow((HWND)_hwnd);
//    }

}

void LuaControl::sleep(unsigned long usecs) {
    QThread::sleep(usecs);
}

void LuaControl::msleep(unsigned long msecs) {
    QThread::msleep(msecs);
}


void LuaControl::mouseMoveToPoint(int x, int y) {
    SetCursorPos(x,y);
}


void LuaControl::mouseMoveToPointClick(int x, int y) {
    activeWindow();
    sKeyMouseMgr.MouseMoveToPointClick(x,y);
}

void LuaControl::keyDown(int key) {
    activeWindow();
    sKeyMouseMgr.keyDown(key);
    qDebug() << "Hwnd" <<QString::number(_hwnd,16) <<"key down: " << key;
}

void LuaControl::keyUp(int key) {
    activeWindow();
    sKeyMouseMgr.keyUp(key);
    qDebug() << "Hwnd" <<QString::number(_hwnd,16) <<"key up: " << key;
}

void LuaControl::keyLongPress(int vkey, long sec) {
    activeWindow();
    sKeyMouseMgr.keyLongPress(vkey,sec);
    qDebug() << "Hwnd" <<QString::number(_hwnd,16) <<"key up: " << vkey;
}



POINT LuaControl::findPicPos(char const *szfile) {
    auto target = cv::imread(szfile);
    return gm::CVHelper::findPicPos((HWND) _hwnd, target);
}

POINT LuaControl::findPicPosScreen(const char *szfile) {
    auto p = findPicPos(szfile);
    ClientToScreen((HWND)_hwnd,&p);
    return p;
}

