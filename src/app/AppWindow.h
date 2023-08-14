//
// Created by c0518535x on 2023/7/14.
//

#ifndef GAMELUA_APPWINDOW_H
#define GAMELUA_APPWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QComboBox>

#include "leptonica/allheaders.h"

#include "WindowMgr.h"
#include "WinIoMgr.h"
#include "ScreenMgr.h"
#include "BitMapMem.h"

class AppWindow : public QMainWindow {
    Q_OBJECT
private:
    QPushButton * testBtn;
    QVBoxLayout * mainLayout;
    QFrame * mainFrame;
    QComboBox* comboBox;
    HWND wind;
    HWND hCurrentCtrl;

public:
    AppWindow(QWidget *parent = nullptr);
    ~AppWindow();
    void doLayout();

public slots:
    void testFunc();
    void ctrlChanged(int index);
};


#endif //GAMELUA_APPWINDOW_H
