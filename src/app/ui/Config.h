//
// Created by c0518535x on 2023/8/7.
//

#ifndef GAMELUA_CONFIG_H
#define GAMELUA_CONFIG_H

/*
 *
 * 使用命名空间作为程序的配置
 */

#include "AppMain.h"

namespace cfg{
    // app信息
    const static char* const  APP_NAME = "GameLua (多开版)";
    const static char* const  APP_AUTHOR = "foxgiker";
    const static char* const APP_VERSION = "1.0.0";
    const static int APP_WIDTH = 760;
    const static int APP_HEIGHT = 480;

    const static QString PATH_DATA_FLODER  = QString::fromLocal8Bit("./data");
    const static QString PATH_FILE_ACCOUNT  = QString::fromLocal8Bit("./data/account.csv");
    const static QString PATH_EXE_TARGET = QString::fromLocal8Bit("H:/PGP/games/WLWZ/element/elementclient.exe");
}

// 向前申明，解决循环依赖
class AppMain;

namespace global{
    const static AppMain*  MAIN_WINDOW = nullptr;   // 全局主窗口
}



#endif //GAMELUA_CONFIG_H
