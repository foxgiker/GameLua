//
// Created by c0518535x on 2023/7/14.
//

#ifndef GAMELUA_APPWINDOW_H
#define GAMELUA_APPWINDOW_H

#include <QMainWindow>
#include "lua.h"


class AppWindow : public QMainWindow {
    Q_OBJECT
public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();
private:

};


#endif //GAMELUA_APPWINDOW_H
