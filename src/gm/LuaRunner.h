//
// Created by c0518535x on 2023/8/8.
//

#ifndef GAMELUA_LUARUNNER_H
#define GAMELUA_LUARUNNER_H

#include <QObject>
#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>
#include <windows.h>
#include <tchar.h>
#include <KeyMouseMgr.h>
#include "LuaControl.h"
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"


class LuaRunner: public QThread  {

private:
    lua_State * _L;   // lua执行器
    uint _hwnd;          // 游戏窗口
    HANDLE _hProcess = NULL ;  //  游戏的窗口所属进程
    HANDLE _hThread = NULL ;    // 注入所用的远程线程
    HANDLE _hToken = NULL;      // 提权时使用的令牌句柄
    LPVOID _lpPathAddr = NULL;  // 远程进程中分配的空间地址
    void doBind();
    void runLua();
    BOOL EnableDebugPrivilege();
protected:
    void run() override ;
public:
    LuaRunner(uint hwnd,QString szFile,QObject* parent);
    ~LuaRunner();
    void InjectWindowProcess(HWND hnd);
    void inject_DLL(TCHAR *dllPath, HANDLE process);
    void testInject();
};


#endif //GAMELUA_LUARUNNER_H
