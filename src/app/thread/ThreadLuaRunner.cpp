//
// Created by c0518535x on 2023/8/8.
//

#include "ThreadLuaRunner.h"


ThreadLuaRunner::ThreadLuaRunner(HWND hwnd, QObject *parent):QThread(parent) {
    sWindowMgr.EnumWindowCtrls(hwnd);
}

ThreadLuaRunner::~ThreadLuaRunner(){
    CloseHandle(_main_hwnd);
}

void ThreadLuaRunner::run() {

}