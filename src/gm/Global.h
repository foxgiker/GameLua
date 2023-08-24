//
// Created by c0518535x on 2023/8/21.
//

#ifndef GAMELUA_GLOBAL_H
#define GAMELUA_GLOBAL_H

#include <windows.h>
#include <QMap>

namespace gb{
    static QMap<HWND,unsigned long long> windows = {};
}


#endif //GAMELUA_GLOBAL_H
