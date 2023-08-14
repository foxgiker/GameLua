//
// Created by c0518535x on 2023/8/10.
//

#ifndef GAMELUA_CVHELPER_H
#define GAMELUA_CVHELPER_H

#include <Windows.h>
#include <QDebug>
#include <opencv2/opencv.hpp>

namespace gm {
    struct Point{
        int x;
        int y;
    };

    class CVHelper {
    private:
        // 传入一个位图句柄，但是不会操作hbMP
        static cv::Mat createMat(HBITMAP hBmp);
    public:
        // 截取窗口指定位置
        static cv::Mat capturePic(HWND,int x, int y, int cx, int cy);
        // 整个窗口截图
        static cv::Mat captureClient(HWND hwnd = nullptr);
        // 查找图片在窗口图片中的位置。
        static Point findPicPos(HWND hwnd,cv::Mat);

    };

}


#endif //GAMELUA_CVHELPER_H
