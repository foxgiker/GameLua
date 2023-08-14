/*
 * Created by c0518535x on 2023/8/8.
 *
*/

#ifndef GAMELUA_LUAREGISTER_H
#define GAMELUA_LUAREGISTER_H

#include <Windows.h>
#include "lua.hpp"
#include <QDebug>
#include "WinIoMgr.h"




extern "C"{
/**
 * 注册虚拟键盘消息相关函数  km
 */

int sendMessage(lua_State *L);
int keyDown(lua_State *L);
int sendChar(lua_State *L);





/*
 * 注册winio驱动级键盘模拟消息 winio
 */



/*
 * 注册屏幕视频的相关函数  sc
 */


/*定义函数表*/
static const luaL_Reg km_lib[] = {
        {"sendMessage",sendMessage},
        {"keyDown",keyDown},
        {"sendChar",sendChar},
        {NULL,NULL}
};


/*创建一个新库*/
int luaopen_km(lua_State* L);

};





#endif //GAMELUA_LUAREGISTER_H
