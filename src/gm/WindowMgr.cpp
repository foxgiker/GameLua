//
// Created by c0518535x on 2023/7/25.
//


#include <QDebug>
#include <iostream>
#include "WindowMgr.h"


namespace gm{

    WindowMgr::WindowMgr() {
        printf("Singleton Create\n");
    }

    WindowMgr::~WindowMgr() {
        printf("Single destory!\n");
    }

    HWND WindowMgr::openProcess(char *path) {
        std::string process_path = "C:\\Windows\\System32\\win32calc.exe";

        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        bool bRet = CreateProcess(
                      L"C:\\Windows\\System32\\notepad.exe",
                      nullptr,
                      nullptr,
                      nullptr,
                      FALSE,
                      0,
                      nullptr,
                      nullptr,
                      &si, &pi
                      );

            Sleep(1000);
            auto hWnd = FindWindow(nullptr,L"无标题 - 记事本");
            SetWindowPos(hWnd,HWND_TOPMOST,0,0,600,600, SWP_ASYNCWINDOWPOS);

            return hWnd;
            //RECT wndRect;
            //GetWindowRect(hWnd,&wndRect);
            //printf("window size %d %d", wndRect.right, wndRect.bottom);
            //PostMessage(hWnd,WM_MOVE,0,0);
            //PostMessage(hWnd,WM_KEYDOWN,(WPARAM)0x35,0xC02C0001);

            // https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
            //PostMessage(hWnd,WM_CHAR,(WPARAM)0x35,0x80000001);
    }



    BOOL CALLBACK WindowMgr::EnumChildProc(HWND hwnd, LPARAM lParam) {
        char szTitle[MAX_PATH] = {0};
        char szClass[MAX_PATH] = {0};
        int nMaxCount = MAX_PATH;

        LPSTR lpClassName = szClass;
        LPSTR lpWindowName = szTitle;

        GetWindowTextA(hwnd, lpWindowName, nMaxCount);
        GetClassNameA(hwnd, lpClassName, nMaxCount);

//        std::cout << "window handle: " << hwnd << " window name: "
//                  << lpWindowName << " class name " << lpClassName <<std::endl;

        WindowCtrl ctrl {hwnd,hwnd,szTitle,szClass};
        auto that =  reinterpret_cast<WindowMgr*>(lParam);
        that->controls.append(ctrl);
        return true;
    }



    void WindowMgr::EnumWindowCtrls(HWND hwnd) {
        char szTitle[MAX_PATH] = {0};
        char szClass[MAX_PATH] = {0};
        int nMaxCount = MAX_PATH;

        LPSTR lpClassName = szClass;
        LPSTR lpWindowName = szTitle;
        GetWindowTextA(hwnd, lpWindowName, nMaxCount);
        GetClassNameA(hwnd, lpClassName, nMaxCount);

        std::cout << "[Main window] window handle: " << hwnd << " window name: "
             << lpWindowName << " class name " << lpClassName <<std::endl;

        controls.clear();
        WindowCtrl ctrl {hwnd,0,szTitle,szClass};
        controls.append(ctrl);

        EnumChildWindows(hwnd,&WindowMgr::EnumChildProc, reinterpret_cast<LPARAM>(this));
    }


    QList<WindowCtrl> WindowMgr::QueryWindows() {
        controls.clear();
        EnumWindows(&WindowMgr::EnumChildProc,reinterpret_cast<LPARAM>(this));
        return controls;
    }

}