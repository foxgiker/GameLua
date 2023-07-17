/*
    Created by c0518535x on 2023/7/14.
*/

#include <QApplication>
#include "AppWindow.h"
#include "lua.hpp"


lua_State * LUA;

 int luaAdd(int x, int y){
    int sum;
    lua_getglobal(LUA,"add");
    lua_pushnumber(LUA,x);
    lua_pushnumber(LUA,y);
    lua_call(LUA,2,1);  // 2个参数一个 返回值
    sum = (int) lua_tonumber(LUA,-1); // 获取结果
    lua_pop(LUA,1); // 清除返回值，弹出栈
    return sum;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppWindow app;
    app.show();

    // Qt调用lua脚本
    LUA = luaL_newstate(); // 新建lua解释器
    luaL_openlibs(LUA); //载入lua基础库
    luaL_dofile(LUA, "../scripts/lua/ui.lua"); // 执行lua脚本

    int sum = luaAdd(10, 20);
    qDebug() << "sum: " << sum;
    lua_close(LUA);
    return a.exec();
}
