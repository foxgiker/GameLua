//
// Created by c0518535x on 2023/7/28.
//

#ifndef GAMELUA_WINIOMGR_H
#define GAMELUA_WINIOMGR_H

#include "winio/winio.h"

namespace gm{

    const DWORD KBC_CMD =  0x64;
    const DWORD KBC_DATA = 0X60;

    /**
     * WinIo 单例对象
     */
    class WinIoMgr {
    private:
        WinIoMgr();
    public:
        WinIoMgr(const  WinIoMgr& ) = delete;
        WinIoMgr& operator=(const WinIoMgr) =delete;

        WinIoMgr(WinIoMgr&&) =delete;
        WinIoMgr* operator=(WinIoMgr**) =delete;

        ~WinIoMgr();
        static WinIoMgr& instance(){
            static WinIoMgr _instance ;  // C++11里 类中的静态方法内的局部静态变量可以保证原子性和线程安全。
            return _instance;
        }

        void KBCWait4IBE();
        void kDown(DWORD key);
        void kUp(DWORD key);

    };


}
#define sWinIoMgr gm::WinIoMgr::instance()
#endif //GAMELUA_WINIOMGR_H
