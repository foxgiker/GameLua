//
// Created by c0518535x on 2023/8/7.
//

#ifndef GAMELUA_QTABLEWIND_H
#define GAMELUA_QTABLEWIND_H

#include <QTableWidget>
#include <QFile>
#include <QHeaderView>

#include "predefine.h"
#include "Config.h"
#include "CVHelper.h"
#include "ScreenMgr.h"
#include "LuaRunner.h"
#include "OCRHelper.h"
#include "WindowMgr.h"
#include "Global.h"


class QTableWind : public QTableWidget{
    Q_OBJECT
private:
    QStringList headLabels = {"HWND","运行状态","账号","昵称","脚本"};
    void setHeaders();
    //加载账号信息
    void loadAccount();
    QList<Account> accounts = {};
public:
    QTableWind(QWidget *parent= nullptr);
    ~QTableWind();

public slots:
    void cellDBClick(int row, int col);
signals:
    void log(LOG_TYPE,QString msg);
};


#endif //GAMELUA_QTABLEWIND_H
