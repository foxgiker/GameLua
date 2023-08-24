/*
================================================
 Created by c0518535x on 2023/7/25
 游戏窗口管理

================================================
 */

#ifndef GAMELUA_WINDOWMGR_H
#define GAMELUA_WINDOWMGR_H

#include <QList>


#include <cstdio>
#include <windows.h>
#include <processthreadsapi.h>
#include <winuser.h>
#include <string>
#include "WindowMgr.h"

namespace gm {


struct WindowCtrl{
    HWND hwnd;  // 句柄
    HWND phwnd;  // 父句柄
    QString title;
    QString clazz;
};


class WindowMgr {
private:
    WindowMgr();   // 声明为私有，保证不会通过构造函数来创建类
    //遍历窗口的回调函数， 当作为类的成员函数时，必须为静态！，通过lparam来传递this
    static BOOL EnumChildProc(HWND hwnd, LPARAM lParam);

public:
    ~WindowMgr();
    WindowMgr(WindowMgr &&) = delete;  //移除移动构造函数
    WindowMgr(const WindowMgr&) = delete; //移除复制构造函数
    WindowMgr & operator=(const WindowMgr&) =delete ; // 移除复制赋值
    WindowMgr * operator=(WindowMgr &&) =delete; // 移除移动赋值

    static WindowMgr& instance(){
        static WindowMgr _instance ;  // C++11里 类中的静态方法内的局部静态变量可以保证原子性和线程安全。
        return _instance;
    }
    //用于保存遍历的窗体控件信息
    QList<WindowCtrl> controls;
    HWND openProcess(char path[]);
    // 遍历指定hwnd的窗口上的所有控件，不同的控件接受的消息不同
    void EnumWindowCtrls(HWND hwnd);

    QList<WindowCtrl> QueryWindows();

};

} // gm

#define sWindowMgr   gm::WindowMgr::instance()

#endif //GAMELUA_WINDOWMGR_H
