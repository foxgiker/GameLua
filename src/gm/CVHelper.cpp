//
// Created by c0518535x on 2023/8/10.
//

#include "CVHelper.h"


cv::Mat gm::CVHelper::createMat(HBITMAP hBmp) {
    BITMAP bmp;
    // 从bitmap指针中提取位图信息
    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);
    PBITMAPINFO pbmi = nullptr;  //位图头信息
    LPBYTE _lpBits = nullptr;         //位图数据: 可能有调色板

    WORD _cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (_cClrBits == 1)
        _cClrBits = 1;
    else if (_cClrBits <= 4)
        _cClrBits = 4;
    else if (_cClrBits <= 8)
        _cClrBits = 8;
    else if (_cClrBits <= 16)
        _cClrBits = 16;
    else if (_cClrBits <= 24)
        _cClrBits = 24;
    else _cClrBits = 32;

    // 分配结构内存
    if (_cClrBits < 24)
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                                        sizeof(BITMAPINFOHEADER) +
                                        sizeof(RGBQUAD) * (1<< _cClrBits));
    else
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                                        sizeof(BITMAPINFOHEADER));
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;


    // 小于24位的位图，要设置调色板。 调色板大小就是 2^n ，n为位数的大小
    int wb = ((pbmi->bmiHeader.biWidth * _cClrBits +31) & ~31) / 8;
    if (_cClrBits < 24) pbmi->bmiHeader.biClrUsed = (1<<_cClrBits);
    pbmi->bmiHeader.biCompression = BI_RGB; //不使用压缩标志
    // 计算位图数据大小： 设置每行的边界、并以DWORD8位对齐
    pbmi->bmiHeader.biSizeImage = wb * pbmi->bmiHeader.biHeight;
    pbmi->bmiHeader.biClrImportant = 0;


    PBITMAPINFOHEADER pbih = (PBITMAPINFOHEADER) pbmi;   //位图文件头
    _lpBits = (LPBYTE) LocalAlloc(GMEM_FIXED, pbih->biSizeImage);
    // 从系统获取位图颜色表和位图数据： 这里没有做异常处理
    GetDIBits(GetWindowDC(GetDesktopWindow()), hBmp, 0, (WORD) pbih->biHeight, _lpBits, pbmi,DIB_RGB_COLORS);



    // 由于 bmp文件的数据是从下到上的， 但是Mat的是从上向下的，所以要把数据按照行进行翻转
    LPBYTE area = (LPBYTE)LocalAlloc(LMEM_ZEROINIT,pbmi->bmiHeader.biSizeImage);
    int dLine = pbmi->bmiHeader.biSizeImage / pbmi->bmiHeader.biHeight;
    for (int i = 0; i < pbmi->bmiHeader.biHeight; ++i) {
        LPBYTE psrc = _lpBits + i * dLine;
        LPBYTE pdest = area + pbmi->bmiHeader.biSizeImage - (i+1) * dLine;
        CopyMemory(pdest,psrc,dLine);
    }
    qDebug() << "bits: " << _cClrBits;
    // windwos系统下的api截图都是4通道的
    cv::Mat img(cv::Size(pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight),CV_8UC(_cClrBits /8), area,wb);
    // 为了跟系统的截图工具等保持一致，移除透明度， 这里可能有点风险，因为不同的电脑api截图不一定都是4通道的， 先这样吧
    cv::cvtColor(img,img,cv::COLOR_RGBA2BGR);
    LocalFree(pbmi);
    LocalFree(_lpBits);
    // area 被mat接管， 不用释放，否则会有bug
    return img;
}

cv::Mat gm::CVHelper::capturePic(HWND hwnd, int x, int y, int width, int height) {

    //测试 窗口区域举行 (30,25)   (15,40)
    // 在截图之前最好让窗口显示一次
    SetWindowPos(hwnd,HWND_TOP,0,0,0,0,SWP_NOSIZE  |SWP_SHOWWINDOW);
    HDC hdcSrc =  GetDC(hwnd);  // 包含标题栏等非客户区
    HDC hdcDest = CreateCompatibleDC(hdcSrc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc,width,height);

    // 选取位图到新的DC中
    HBITMAP hOld =  (HBITMAP) SelectObject(hdcDest,hBitmap);
    // 这里截取了指定位置的特定大小的图片
    BitBlt(hdcDest,0,0,width,height,hdcSrc,x,y,SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hdcDest, hOld);

    // 逻辑处理 hBitmap
    auto img = createMat(hBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcDest);
    ReleaseDC(hwnd,hdcSrc);
    return img;
}

cv::Mat gm::CVHelper::captureClient(HWND hwnd) {

    if(hwnd == nullptr) hwnd = GetDesktopWindow();
    HWND topHwnd = GetForegroundWindow();
    if(hwnd != topHwnd){
        SetWindowPos(hwnd,HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE |SWP_SHOWWINDOW);
        SetActiveWindow(hwnd);
    }

    RECT rect; //窗口区域举行
    GetClientRect(hwnd,&rect);
    // 截图的大小
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    qDebug() <<"captureClient屏幕取图大小：" << width << "," << height;
    auto img =  capturePic(hwnd,0,0,width,height);
    return img;
}

POINT gm::CVHelper::findPicPos(HWND hwnd, cv::Mat t) {
    auto p = captureClient(hwnd);
    cv::Mat image_matched;

    cv::imwrite("p.bmp",p);
    cv::imwrite("t.bmp",t);

    cv::matchTemplate(p,t,image_matched,cv::TM_CCOEFF_NORMED);

    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    //寻找最佳匹配位置
    cv::minMaxLoc(image_matched, &minVal, &maxVal, &minLoc, &maxLoc);

    qDebug() << "最佳匹配位置： " << maxLoc.x << "," << maxLoc.y;
    qDebug() << "最差匹配位置： " << minLoc.x << "," << minLoc.y;
    POINT point{maxLoc.x,maxLoc.y};
    return point;

    // 把找到的位置圈起来
//    cv::Mat image_color;
//    cv::cvtColor(p, image_color, cv::COLOR_GRAY2BGR);
//    cv::circle(image_color,
//               cv::Point(maxLoc.x + t.cols/2, maxLoc.y + t.rows/2),
//               20,
//               cv::Scalar(0, 0, 255),
//               2,
//               8,
//               0);

}