//
// Created by c0518535x on 2023/8/8.
//

#include "LuaRegister.h"


int sendMessage(lua_State *L) {
    //检查栈中的参数是否合法，1表示Lua调用时的第一个参数(从左到右)，依此类推。
    //如果Lua代码在调用时传递的参数不为number，该函数将报错并终止程序的执行。
    int hwnd = luaL_checknumber(L, 1);
    int type = luaL_checknumber(L, 2);
    int w = luaL_checknumber(L, 3);
    int l = luaL_checknumber(L, 4);
    //将函数的结果压入栈中。如果有多个返回值，可以在这里多次压入栈中。
    PostMessage((HWND)hwnd,type,(WPARAM) w, (LPARAM) l);
    //返回值用于提示该C函数的返回值数量，即压入栈中的返回值数量。
    return 0;
}

int keyDown(lua_State *L) {
    int hwnd = luaL_checknumber(L, 1);
    int code = luaL_checknumber(L,2);

    //SendMessage((HWND)hwnd,WM_KEYDOWN,(WPARAM)code,(LPARAM)1);
    SetActiveWindow((HWND)hwnd);
    sWinIoMgr.kDown(code);
    qDebug() << "call keyDown: hwnd " << hwnd  << "code: " << code;
    return 0;
}

int sendChar(lua_State *L){
    return 0;
}


/*创建一个新库*/
int luaopen_km(lua_State* L) {
    luaL_newlib(L, km_lib);
    // 附加虚拟键盘码
    return 1;
}