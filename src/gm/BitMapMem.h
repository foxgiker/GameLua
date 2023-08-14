/*
 * ======================================================
 * Created by c0518535x on 2023/8/2.
 * 本项目中统一使用bmp格式的图片，该类作为bmp文件在内存中的表示。
 *
 * 注意点：
 * 1. 由于结构体或者类成员在内存中不一定是连续的，所以在生成位图的mem时，要重新分配内存进行组装
 * 参考：
 * [1]. https://learn.microsoft.com/en-us/windows/win32/gdi/storing-an-image
 * [2]. https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170
 * ======================================================
*/

#ifndef GAMELUA_BITMAPMEM_H
#define GAMELUA_BITMAPMEM_H

#include <windows.h>
#include<opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>

#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>

namespace gm {
    class BitMapMem {
    private:
        WORD _cClrBits = 32;
        PBITMAPFILEHEADER pbfdr = nullptr;  //文件头信息
        PBITMAPINFO pbmi = nullptr;  //位图头信息
        LPBYTE _lpBits = nullptr;         //位图数据: 可能有调色板
        char *_mem = nullptr;          // 完整的位图信息
        void CreateBitmapInfo(HBITMAP);  //从系统句柄中获取位图信息
        void CreateBitmapFileHeader(PBITMAPINFO);   // 创建文件信息

        // 从文件中加载
        void load(const WCHAR *szFile);

        // 从一个系统句柄中加载
        void load(HBITMAP hBmp);
    public:

        BitMapMem();
        ~BitMapMem();
        BitMapMem(BitMapMem &&);
        BitMapMem(const BitMapMem &);
        BitMapMem operator=(BitMapMem &&);
        BitMapMem operator=(const BitMapMem &);

        BitMapMem(HBITMAP);
        BitMapMem(const WCHAR *szFile);
        // 完整的mem信息
        char *mem() { return _mem; };
        //保存到文件
        void save(LPCWSTR szFile);
        // 转换成opencv的Mat
        cv::Mat toMat();

        char* toString();

        void preprocess();  // 预处理

    };

}
#endif //GAMELUA_BITMAPMEM_H
