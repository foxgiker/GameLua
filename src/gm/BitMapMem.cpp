//
// Created by c0518535x on 2023/8/2.
//

#include <QDebug>
#include "BitMapMem.h"

gm::BitMapMem::~BitMapMem() {
    if(_mem != nullptr) LocalFree(_mem);
    if(_lpBits != nullptr) LocalFree(_lpBits);
    if(pbmi!= nullptr) LocalFree(pbmi);
    if(pbfdr != nullptr) LocalFree(pbfdr);
}

gm::BitMapMem::BitMapMem()  {}

gm::BitMapMem::BitMapMem(HBITMAP hbp) {
    load(hbp);
}

gm::BitMapMem::BitMapMem(const WCHAR *szFile) {
    load(szFile);
}



/*
 * 从位图指针中获取位图信息。
 */
void gm::BitMapMem::CreateBitmapInfo(HBITMAP hBmp) {
    BITMAP bmp;
    // 从bitmap指针中提取位图信息
    GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);
    _cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
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
    if (_cClrBits < 24) pbmi->bmiHeader.biClrUsed = (1<<_cClrBits);
    pbmi->bmiHeader.biCompression = BI_RGB; //不使用压缩标志
    // 计算位图数据大小： 设置每行的边界、并以DWORD8位对齐
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * _cClrBits +31) & ~31) /8 * pbmi->bmiHeader.biHeight;
    pbmi->bmiHeader.biClrImportant = 0;
}

void gm::BitMapMem::load(HBITMAP hBmp) {
    CreateBitmapInfo(hBmp);  // 创建位图信息
    PBITMAPINFOHEADER pbih = (PBITMAPINFOHEADER) pbmi;   //位图文件头

    _lpBits = (LPBYTE) LocalAlloc(GMEM_FIXED, pbih->biSizeImage);
    // 从系统获取位图颜色表和位图数据： 这里没有做异常处理
    GetDIBits(GetWindowDC(GetDesktopWindow()), hBmp, 0, (WORD) pbih->biHeight, _lpBits, pbmi,DIB_RGB_COLORS);

    //计算文件头
    pbfdr = (PBITMAPFILEHEADER) LocalAlloc(LMEM_ZEROINIT,sizeof(BITMAPFILEHEADER));
    pbfdr->bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
    pbfdr->bfReserved1 = 0;
    pbfdr->bfReserved2 = 0;
    pbfdr->bfOffBits = sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);
    pbfdr->bfSize = pbfdr->bfOffBits + pbih->biSizeImage;

    // 把所有的都写入到mem中
    _mem = (char*) LocalAlloc(LMEM_ZEROINIT,pbfdr->bfSize);
    CopyMemory(_mem,pbfdr,sizeof(BITMAPFILEHEADER));  //复制文件头
    char* addr = _mem + sizeof(BITMAPFILEHEADER);
    CopyMemory(addr,pbmi, pbmi->bmiHeader.biSize);    //复制位图头
    addr += pbmi->bmiHeader.biSize;
    CopyMemory(addr,_lpBits, pbih->biSizeImage);
    preprocess();
}

void gm::BitMapMem::load(const WCHAR *szFile) {
    OFSTRUCT ofs;
    HANDLE hfile = CreateFile(szFile,GENERIC_READ,
                             FILE_SHARE_READ,
                             NULL,
                             OPEN_EXISTING,        //打开已存在的文件
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    int dwBits;
    // ReadFile 以后，会自动指针会自动向后移动！！
    if ( pbfdr == nullptr) pbfdr = (PBITMAPFILEHEADER ) LocalAlloc(LMEM_ZEROINIT,sizeof(BITMAPFILEHEADER ));
    ReadFile(hfile, pbfdr, sizeof(BITMAPFILEHEADER), reinterpret_cast<LPDWORD>(&dwBits), NULL);
    qDebug() <<   "file header size: " <<  pbfdr->bfType;


    SetFilePointer(hfile,0,NULL,FILE_BEGIN);
    if(_mem == nullptr) _mem = static_cast<char *>(LocalAlloc(LMEM_ZEROINIT, pbfdr->bfSize));
    ReadFile(hfile,_mem,pbfdr->bfSize,reinterpret_cast<LPDWORD>(&dwBits),NULL);


    SetFilePointer(hfile,sizeof(BITMAPFILEHEADER),NULL,FILE_BEGIN);
    if(pbmi == nullptr) pbmi = (PBITMAPINFO)LocalAlloc(LMEM_ZEROINIT,sizeof(BITMAPINFO));
    ReadFile(hfile,&pbmi->bmiHeader,sizeof(BITMAPINFOHEADER),reinterpret_cast<LPDWORD>(&dwBits),NULL);
    _cClrBits = pbmi->bmiHeader.biBitCount;

    SetFilePointer(hfile,pbfdr->bfOffBits,NULL,FILE_BEGIN);
    if(_lpBits == nullptr) _lpBits = (LPBYTE) LocalAlloc(LMEM_ZEROINIT,pbmi->bmiHeader.biSizeImage);
    ReadFile(hfile,_lpBits,pbmi->bmiHeader.biSizeImage,reinterpret_cast<LPDWORD>(&dwBits),NULL);

    CloseHandle((HANDLE)hfile);
    qDebug() << "file load finished!";
}

// 保存到文件
void gm::BitMapMem::save(LPCWSTR szFile) {
    DWORD dwTmp;
    HANDLE hf = CreateFile(szFile,
                           GENERIC_WRITE,
                           0,
                           NULL,
                           CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL,
                           NULL);
    qDebug() << "create finished!";
    // 写入文件信息
    WriteFile(hf,_mem,pbfdr->bfSize,&dwTmp,NULL);
    CloseHandle(hf);
    qDebug() << "write finished!";
}


cv::Mat gm::BitMapMem::toMat() {
    // 每行的像素所占据的字节大小（包括边距）
    int wb = ((pbmi->bmiHeader.biWidth * _cClrBits +31) & ~31) / 8;
    // 由于 bmp文件的数据是从下到上的， 但是Mat的是从上向下的，所以要把数据按照行进行翻转
    LPBYTE area = (LPBYTE)LocalAlloc(LMEM_ZEROINIT,pbmi->bmiHeader.biSizeImage);
    int dLine = pbmi->bmiHeader.biSizeImage / pbmi->bmiHeader.biHeight;
    for (int i = 0; i < pbmi->bmiHeader.biHeight; ++i) {
        LPBYTE psrc = _lpBits + i * dLine;
        LPBYTE pdest = area + pbmi->bmiHeader.biSizeImage - (i+1) * dLine;
        CopyMemory(pdest,psrc,dLine);
    }
    cv::Mat cv_img(cv::Size(pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight),CV_8UC(_cClrBits / 8), area,wb);
    return cv_img;
}


char *gm::BitMapMem::toString() {
    cv::Mat mg = toMat();
    cv::Mat mg1, mg2, mg3,mg4,mg5,mg6,t;
    cv::cvtColor(mg, mg3, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(mg3, mg2, cv::Size(3, 3), 0.5,0.5);
    cv::threshold(mg2,t,180,255,cv::THRESH_BINARY_INV );
    cv::imwrite("ocr-pre.bmp",t);

    tesseract::TessBaseAPI *ocr = new tesseract::TessBaseAPI();
    ocr->Init("./data", "eng+chi_sim_vert");

    ocr->SetImage(t.data,t.size().width,t.size().height,t.channels(),t.step1());
    auto txt = ocr->GetUTF8Text();
    qDebug() << "ocr result: " << txt;
    ocr->End();
    return txt;
}


void gm::BitMapMem::preprocess() {
    cv::Mat mg = toMat();
    cv::Mat mg1, mg2, mg3,mg4,mg5,mg6,imgGray;


    //cv::Laplacian(cv_img,mg1,CV_8U,1,1,0,cv::BORDER_ISOLATED);
    //cv::convertScaleAbs(cv_img,mg1);
   // cv::GaussianBlur(cv_img, mg2, cv::Size(1, 1), 0.2,0.2);

    cv::cvtColor(mg, mg3, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(mg3, mg2, cv::Size(3, 3), 0.5,0.5);
    cv::threshold(mg2,mg4,180,255,cv::THRESH_BINARY_INV );
    //cv::imwrite("g_test1.bmp",mg);
    //cv::blur(cv_img,imgGray,cv::Size(4,4));
    //cv::adaptiveThreshold(mg3,mg4,255,cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY ,3,2);
    //cv::threshold(mg3,mg4,0,255,cv::THRESH_BINARY |cv::THRESH_OTSU  );


}

