//
// Created by c0518535x on 2023/8/7.
//

#ifndef GAMELUA_PREDEFINE_H
#define GAMELUA_PREDEFINE_H

#include <QString>
#include <QMap>

enum LOG_TYPE {
    LOG_INFO,
    LOG_DEBUG,
    LOG_WARNING,
    LOG_ERROR
};

enum AC_STATUS{
    AC_NONE,
    AC_LOGIN,
    AC_PREPARE,
    AC_STOP,
    AC_RUN
};

const static QMap<AC_STATUS,QString> ac_status_dict = {
        {AC_NONE,QString::fromLocal8Bit("未知")},
        {AC_LOGIN,QString::fromLocal8Bit("登录中")},
        {AC_PREPARE,QString::fromLocal8Bit("脚本准备中")},
        {AC_STOP,QString::fromLocal8Bit("脚本已停止")},
        {AC_RUN,QString::fromLocal8Bit("运行中")},
} ;

struct Account{
    QString account = "";
    QString nickname = "";
    QString password = "";
    QString server = "";      // 区服
    QString script = "";      // 执行的脚本
    AC_STATUS status = AC_STATUS::AC_NONE;
};


#endif //GAMELUA_PREDEFINE_H
