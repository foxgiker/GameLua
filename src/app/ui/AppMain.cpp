//
// Created by c0518535x on 2023/8/7.
//

#include "AppMain.h"

QString AppMain::getTitle() {
    return QString("%1-%2-%3").arg(cfg::APP_NAME,cfg::APP_VERSION,cfg::APP_AUTHOR);
}

void AppMain::doInit() {
    _w_status_bar = new QStatusBar(this);
    _w_log = new QTextBrowser(this);
    _w_tb_splitter = new QSplitter(this);
    _w_lr_splitter = new QSplitter(this);


    _w_top_frame = new QFrame(this);
    _w_top_frame_layout = new QHBoxLayout(_w_top_frame);
    _w_windows_group = new QGroupBox(this);
    _w_status_group = new QGroupBox(this);
    _w_windows_group_layout = new QVBoxLayout(_w_windows_group);
    _w_status_group_layout = new QVBoxLayout(_w_status_group);

    _w_windows_table = new QTableWind(this);

    _w_main = new QFrame(this);
    _w_main_layout = new QVBoxLayout(_w_main);



}

void AppMain::doDestory(){
    delete _w_main_layout;
}




void AppMain::doLayout() {
    setWindowTitle(getTitle());
    setStatusBar(_w_status_bar);

    _w_log->document()->setMaximumBlockCount(1000);

    _w_windows_group->setTitle(QString::fromLocal8Bit("窗口列表"));
    _w_windows_group->setLayout(_w_windows_group_layout);
    _w_windows_group_layout->setContentsMargins(0,2,0,0);
    _w_windows_group_layout->addWidget(_w_windows_table);


    _w_status_group->setTitle(QString::fromLocal8Bit("状态信息"));
    _w_status_group->setLayout(_w_status_group_layout);

    _w_status_group->setMinimumWidth(200);
    _w_lr_splitter->addWidget(_w_status_group);
    _w_lr_splitter->addWidget(_w_windows_group);
    _w_lr_splitter->setStretchFactor(0,1);
    _w_lr_splitter->setStretchFactor(1,2);

    _w_top_frame_layout->addWidget(_w_lr_splitter);
    _w_top_frame_layout->setContentsMargins(0,0,0,0);
    _w_top_frame->setLayout(_w_top_frame_layout);

    _w_tb_splitter->setOrientation(Qt::Vertical);
    _w_tb_splitter->addWidget(_w_top_frame);
    _w_tb_splitter->addWidget(_w_log);
    _w_tb_splitter->setStretchFactor(0,4);
    _w_tb_splitter->setStretchFactor(1,1);

    _w_main_layout->addWidget(_w_tb_splitter);

    _w_main->setLayout(_w_main_layout);
    setCentralWidget(_w_main);
    resize(cfg::APP_WIDTH,cfg::APP_HEIGHT);
    showLog(LOG_INFO,"页面布局完成");
}

void AppMain::showLog(LOG_TYPE type, QString msg) {
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateStr = currentDateTime.toString("yyyy.MM.dd hh:mm:ss ddd");

    // 设置文字（样式+内容）
    QString level ;
    switch (type) {
        case LOG_ERROR:
            level = QString("<span color=\"red\">[%1]</span>").arg("ERROR");
            break;
        case LOG_INFO:
            level = QString("<span style=\"color:green\">[%1]</span>").arg("INFO");
            break;
        case LOG_WARNING:
            level = QString("<span color=\"yellow\">[%1]</span>").arg("WARNING");
            break;
        default:
            level = QString("<span color=\"black\">[%1]</span>").arg("ERROR");
            break;
    }
    QString  str = QString("%1-%2: %3").arg(level,currentDateStr,msg);
    _w_log->append(str);
}



void AppMain::doRegister() {
    global::MAIN_WINDOW = this;
}

void AppMain::setEnv() {
    auto lua_path = QString("%1/lua").arg(QDir::currentPath()).toLocal8Bit();

    auto res = qputenv("LUA_PATH",lua_path);

    qDebug() << "set Env"<< QDir::currentPath();
}

AppMain::AppMain() {
    doRegister();
    doInit();
    doLayout();
}

AppMain::~AppMain() {
    doDestory();
}