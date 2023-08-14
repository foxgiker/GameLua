//
// Created by c0518535x on 2023/8/8.
//

#ifndef GAMELUA_LUARUNNER_H
#define GAMELUA_LUARUNNER_H

#include <QObject>
#include <QDebug>
#include <Windows.h>
#include <WinIoMgr.h>
#include "lua.hpp"
#include "LuaBridge/LuaBridge.h"


void test(int hwnd,int code);

class LuaRunner: public QObject {

private:
    lua_State * _L;   // lua执行器
public:
    LuaRunner(HWND hwnd,QString szFile,QObject* parent);
    ~LuaRunner();
    void run();
};


#endif //GAMELUA_LUARUNNER_H
