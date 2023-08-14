//
// Created by c0518535x on 2023/8/7.
//

#include "QTableWind.h"


QTableWind::QTableWind(QWidget *parent) {
    setHeaders();
    loadAccount();
    connect(this,&QTableWidget::cellDoubleClicked,this,&QTableWind::cellDBClick);
    connect(this,&QTableWind::log,(AppMain*) parent,&AppMain::showLog);

}

QTableWind::~QTableWind(){

}

void QTableWind::cellDBClick(int row,int col) {
    emit log(LOG_TYPE::LOG_INFO,QString("点击了 %1行 %2 列").arg(row).arg(col));
    HWND hnd =  FindWindow(NULL,L"Element Client");
    qDebug() <<"hnd: " << hnd;


    //sWindowMgr.EnumWindowCtrls(hnd);
    // 开启线程执行脚本

    LuaRunner luaRunner(hnd,QString("./lua/test.lua"),this);
    luaRunner.run();

//    auto t = gm::CVHelper::capturePic(GetDesktopWindow(),55,10,32,20);
//    cv::imwrite("t.bmp",t);
//    gm::OCRHelper::instance().GetUTF8Text(t);

    //sScreenMgr.CaptureByGdi(hwnd);
}

void QTableWind::loadAccount() {
    QFile file(cfg::PATH_FILE_ACCOUNT);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return ;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        auto item =  line.split(',');
        Account account;
        account.account = item.at(0);
        account.password = item.at(1);
        account.nickname = item.at(2);
        account.server = item.at(3);
        account.script = item.at(4);
        accounts.append(account);
    }
    setRowCount(accounts.size());
    setColumnCount(5);
    for (int i = 0; i < accounts.size(); ++i) {
        auto ac = accounts.at(i);
        setItem(i,0,new QTableWidgetItem(0,QTableWidgetItem::Type));
        setItem(i,1,new QTableWidgetItem (ac_status_dict[ac.status]));
        setItem(i,2,new QTableWidgetItem(ac.account));
        setItem(i,3,new QTableWidgetItem(ac.nickname));
        setItem(i,4,new QTableWidgetItem(QString(ac.script).trimmed()));
    }
}

void QTableWind::setHeaders() {
    setColumnCount(headLabels.size());
    setHorizontalHeaderLabels(headLabels);
}