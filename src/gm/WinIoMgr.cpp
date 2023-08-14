//
// Created by c0518535x on 2023/7/28.
//

#include <QDebug>
#include "WinIoMgr.h"


// 初始化winio
gm::WinIoMgr::WinIoMgr(){
    bool Result = InitializeWinIo();
    if(Result){
        qDebug() <<"WinIon initialized Successfully!";
    }else{
        qDebug() <<"WinIon initialized Failed!";
    }
}

// 清理
gm::WinIoMgr::~WinIoMgr() {
    ShutdownWinIo();
}

void gm::WinIoMgr::KBCWait4IBE() {
    DWORD dwVal=0;
    do
    {
        GetPortVal(KBC_CMD,&dwVal,1);
    }while((&dwVal)&&(0x2)==0);
}

void gm::WinIoMgr::kDown(DWORD key) {
    Sleep(20);    //休息2秒
    KBCWait4IBE(); //等待键盘缓冲区为空
    SetPortVal( 0X64, 0xD2, 1 ); //发送键盘写入命令
    Sleep(10);
    KBCWait4IBE();
    SetPortVal( 0X60, MapVirtualKey(key, 0), 1 ); //写入按键信息,按下键
}

void gm::WinIoMgr::kUp(DWORD key) {
    Sleep(10);
    KBCWait4IBE(); //等待键盘缓冲区为空
    if (SetPortVal(0X64, 0xD2, 1 )) //发送键盘写入命令
        Sleep(10);
    KBCWait4IBE();
    SetPortVal(0X60, (MapVirtualKey(key, 0) | 0x80), 1);//写入按键信息，释放键
}