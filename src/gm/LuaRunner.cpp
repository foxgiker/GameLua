//
// Created by c0518535x on 2023/8/8.
//

#include "LuaRunner.h"

static int vk_0 = 0x30;
static int vk_1 = 0x31;
static int vk_a = 0x41;
static int vk_d = 0x44;
static int vk_s = 0x53;
static int vk_w = 0x57;


void test(int hwnd,int code){

    SetWindowPos((HWND)hwnd,HWND_TOP,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE |SWP_SHOWWINDOW);
    ShowWindow((HWND)hwnd,SW_SHOWNORMAL);
    SetActiveWindow((HWND)hwnd);
    sWinIoMgr.kDown(code);
}


LuaRunner::LuaRunner(HWND hwnd,QString szFile, QObject *parent) {
    _L = luaL_newstate();
    luaL_openlibs(_L); //载入lua基础库

    qDebug() << "hnd: " << hwnd;

    luabridge::setGlobal(_L,(int) hwnd,"hwnd");
    luabridge::setGlobal(_L,&vk_0,"vk_0");
    luabridge::setGlobal(_L,&vk_w,"vk_w");

    luabridge::getGlobalNamespace(_L)
    .beginNamespace("wm")   // 窗口管理
        .addFunction("keyDown", test)
    .endNamespace();

    luaL_dofile(_L,szFile.toLocal8Bit().data());
}

LuaRunner::~LuaRunner(){
    lua_close(_L);
}


void LuaRunner::run() {
    lua_getglobal(_L,"run");  //查找函数名
    lua_call(_L,0,0);  //没有参数，没有返回值
}
