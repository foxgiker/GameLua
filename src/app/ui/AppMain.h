//
// Created by c0518535x on 2023/8/7.
//

#ifndef GAMELUA_APPMAIN_H
#define GAMELUA_APPMAIN_H

#include <QMainWindow>
#include <QDebug>
#include <QString>
#include <QStatusBar>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QSplitter>
#include <QPlainTextEdit>
#include <QFrame>
#include <QTextBrowser>
#include <QStyle>
#include <QDateTime>
#include <QDir>
#include <QtGlobal>


#include "Config.h"
#include "QTableWind.h"

class QTableWind;

class AppMain:public QMainWindow{
    Q_OBJECT
private:
    QStatusBar*           _w_status_bar;               // 底部状态栏
    QGroupBox*            _w_windows_group;            // 窗口列表分组
    QGroupBox*            _w_status_group;             // 右侧状态信息分组
    QVBoxLayout*          _w_windows_group_layout;            // 窗口列表分组布局
    QVBoxLayout*          _w_status_group_layout;             // 右侧状态信息布局
    QTableWind*           _w_windows_table;            // 窗口列表
    QVBoxLayout*          _w_main_layout;                  // 窗口主界面布局
    QSplitter*            _w_tb_splitter;                 // 上下分割栏目
    QSplitter*            _w_lr_splitter;                 // 左右分割栏目
    QTextBrowser*         _w_log;                      // 下方的日志窗口
    QFrame*               _w_main;                     // 主frame
    QFrame*               _w_top_frame;                 // 上面frame
    QHBoxLayout*          _w_top_frame_layout;                 // 上面frame布局


    QString getTitle();
    void doRegister();
    void doInit();
    void doDestory();
    void doLayout();
    void setEnv();
public:
    AppMain();
    ~AppMain();

public slots:
    void showLog(LOG_TYPE,QString msg);   // 显示日志
};


#endif //GAMELUA_APPMAIN_H
