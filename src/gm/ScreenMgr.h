//
// Created by c0518535x on 2023/7/28.
//

#ifndef GAMELUA_SCREENMGR_H
#define GAMELUA_SCREENMGR_H

#include "windows.h"
#include <d3d11.h>
#include <dxgi1_2.h>


#include <QDebug>
#include <QDateTime>
#include <BitMapMem.h>



namespace gm {

    // D3D会尝试支持的功能级别
    const D3D_FEATURE_LEVEL FeatureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3
        };

    // Driver types supported 支持的驱动程序类型
    const D3D_DRIVER_TYPE DriverTypes[] =
        {
            D3D_DRIVER_TYPE_HARDWARE,
            D3D_DRIVER_TYPE_WARP,
            D3D_DRIVER_TYPE_REFERENCE
        };

    //BMP文件格式
    struct BMPFILE{
        BITMAPFILEHEADER hdr;
        BITMAPINFOHEADER idr;
    };



    /*
     * 单例类，用于处理屏幕截图
     */
    class ScreenMgr {
    private:
        ScreenMgr();

    public:
        ~ScreenMgr();

        ScreenMgr(const ScreenMgr &) = delete;
        ScreenMgr(ScreenMgr &&) = delete;

        ScreenMgr &operator=(const ScreenMgr &) = delete;
        ScreenMgr *operator=(ScreenMgr &&) = delete;

        static ScreenMgr &intance() {
            static ScreenMgr _instance;  // C++11里 类中的静态方法内的局部静态变量可以保证原子性和线程安全。
            return _instance;
        }

        PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp);
        // 使用direct 来捕获屏幕
        BOOL CaptureByDx();
        // 使用gid截取指定窗口的截图
        PBITMAPINFO CaptureByGdi(HWND hWnd);
        // 将HBITMAP转化为 BMPFILE文件结构
        BMPFILE CreateBMPFILE(HBITMAP hBMP);
    };

}

#define sScreenMgr gm::ScreenMgr::intance()
#endif //GAMELUA_SCREENMGR_H
