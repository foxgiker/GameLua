//
// Created by c0518535x on 2023/8/8.
//

#ifndef GAMELUA_THREADLUARUNNER_H
#define GAMELUA_THREADLUARUNNER_H

#include <Windows.h>
#include <QThread>
#include "../ui/predefine.h"
#include "../gm/WindowMgr.h"

class ThreadLuaRunner: public QThread {
    Q_OBJECT
private:
    HWND _main_hwnd;
public:
    ThreadLuaRunner(HWND hwnd,QObject* parent = nullptr);  //要使用一个窗口句柄
    ~ThreadLuaRunner();
    void run() override;
public slots:

    signals:
    void log(LOG_TYPE,QString msg);   //打印日志
};


#endif //GAMELUA_THREADLUARUNNER_H
