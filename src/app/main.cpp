/*
    Created by c0518535x on 2023/7/14.
*/

#include <QApplication>
#include "AppWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppWindow app;
    app.show();
    return a.exec();
}
