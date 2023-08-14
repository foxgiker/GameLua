//
// Created by c0518535x on 2023/7/14.
//

#include "AppWindow.h"

AppWindow::AppWindow(QWidget *parent):QMainWindow(parent) {

    qDebug("Hello World");
    doLayout();
}


AppWindow::~AppWindow() noexcept {

}


void AppWindow::doLayout() {
    mainLayout = new QVBoxLayout(this);
    testBtn = new QPushButton(this);
    mainFrame = new QFrame(this);
    comboBox = new QComboBox(this);

    connect(testBtn,&QPushButton::clicked,this,&AppWindow::testFunc);

    this->resize(300,300);
    testBtn->setText("测试");

    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(testBtn);
    mainFrame->setLayout(mainLayout);
    setCentralWidget(mainFrame);

    wind = sWindowMgr.openProcess(nullptr);
    sWindowMgr.EnumWindowCtrls(wind);

    //遍历完成后显示窗口控件
    for (int i = 0; i < sWindowMgr.controls.count(); ++i) {
        auto ctrl = sWindowMgr.controls.at(i);
        auto label = QString("0x%1").arg((uint)ctrl.hwnd, 8, 16, QLatin1Char( '0' )) + "- "+
                ctrl.clazz;
        comboBox->addItem(label,QVariant::fromValue(ctrl.hwnd));
    }

    connect(comboBox,&QComboBox::currentIndexChanged,this,&AppWindow::ctrlChanged);
}

void AppWindow::ctrlChanged(int index) {
    auto c = sWindowMgr.controls.at(index);
    hCurrentCtrl = c.hwnd;
    qDebug() << "选择了" + c.clazz;
}

//void testSendMsg(){
//    auto wParam =(WPARAM) 0x35;
//    SetActiveWindow(wind);
//    for (int i = 0; i < 10; ++i) {
//        Sleep(2000);
///       PostMessage(hCurrentCtrl,WM_CHAR,wParam,0x0);
//        printf("send msg to : %x, key: %x", wind,wParam);
//        sWinIoMgr.KeyDown(0x36);
//        printf(" %d. send msg: 0x36  \n",i);
//        sWinIoMgr.KeyUp(0x36);
//}

void AppWindow::testFunc() {
    //sScreenMgr.CaptureByDx();
    gm::BitMapMem  bmp(L"./data/pet.bmp");
    //bmp.save(L"./hongbak.bmp");
    bmp.toString();

}
