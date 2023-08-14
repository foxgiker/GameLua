/*
 *========================================================
 * Created by c0518535x on 2023/7/28.
 * 描述：主要是定义常用的屏幕截图方法
 *========================================================
*/


#include "ScreenMgr.h"


gm::ScreenMgr::ScreenMgr() {

}

// 析构处理
gm::ScreenMgr::~ScreenMgr() {
    qDebug() <<"ScreenMgr destoried!=====\n";
}

BOOL gm::ScreenMgr::CaptureByDx() {
    qDebug() << "dxgi capture start:"  << QDateTime::currentMSecsSinceEpoch();
    UINT NumDriverTypes = ARRAYSIZE(DriverTypes);
    UINT NumFeatureLevels = ARRAYSIZE(FeatureLevels);

    D3D_FEATURE_LEVEL FeatureLevel;
    ID3D11Device *_pDX11Dev = nullptr;
    ID3D11DeviceContext *_pDX11DevCtx = nullptr;

    HRESULT hr;
    // 遍历设备类型，尝试创建一个代表显示适配器的设备， 如果尝试成功，会写入设备类型和特性标准
    for (UINT index = 0; index < NumDriverTypes; index++)
    {
        hr = D3D11CreateDevice(nullptr,                   //如果为空，使用默认的适配器
                               DriverTypes[index],
                               nullptr, 0,
                               FeatureLevels,
                               NumFeatureLevels,
                               D3D11_SDK_VERSION,
                               &_pDX11Dev,                // 返回- 设备对象
                               &FeatureLevel,         // 返回-支持的特性等级
                               &_pDX11DevCtx);   //返回- 设备上下文

        if (SUCCEEDED(hr)) {
            break;
        }
    }

    IDXGIDevice *_pDXGIDev = nullptr;
    // 获取 DXGI 设备
    hr = _pDX11Dev->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&_pDXGIDev));
    if (FAILED(hr)) {
        return false;
    }

    IDXGIAdapter *_pDXGIAdapter = nullptr;
    //获取 DXGI 适配器
    hr = _pDXGIDev->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&_pDXGIAdapter));
    if (FAILED(hr)) {
        return false;
    }

    UINT i = 0;
    IDXGIOutput *_pDXGIOutput = nullptr;
    // 遍历获取输出
    hr = _pDXGIAdapter->EnumOutputs(i, &_pDXGIOutput);
    if (FAILED(hr)) {
        return false;
    }


    DXGI_OUTPUT_DESC DesktopDesc;
    // 获取输出描述结构
    _pDXGIOutput->GetDesc(&DesktopDesc);

    IDXGIOutput1 *_pDXGIOutput1 = nullptr;
    //请求接口给Output1
    hr = _pDXGIOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&_pDXGIOutput1));
    if (FAILED(hr)) {
        return false;
    }
    // IDXGIOutputDuplication 可以访问和操控复制的桌面图片
    IDXGIOutputDuplication *_pDXGIOutputDup = nullptr;
    // 创建桌面副本
    hr = _pDXGIOutput1->DuplicateOutput(_pDX11Dev, &_pDXGIOutputDup);
    if (FAILED(hr)) {
        return false;
    }


    IDXGIResource *desktopResource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO frameInfo;
    hr = _pDXGIOutputDup->AcquireNextFrame(20, &frameInfo, &desktopResource);
    if (FAILED(hr))
    {
        if (hr == DXGI_ERROR_WAIT_TIMEOUT)
        {
            if (desktopResource) {
                desktopResource->Release();
                desktopResource = nullptr;
            }
            hr = _pDXGIOutputDup->ReleaseFrame();
        }
        else
        {
            return false;
        }
    }

    ID3D11Texture2D *_pDX11Texture = nullptr;
    //查询下一帧暂存缓冲区
    hr = desktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&_pDX11Texture));
    desktopResource->Release();
    desktopResource = nullptr;
    if (FAILED(hr)) {
        return false;
    }

    ID3D11Texture2D *_pCopyBuffer = nullptr;

    D3D11_TEXTURE2D_DESC desc;
    // copy old description 复制旧描述
    if (_pDX11Texture)
    {
        _pDX11Texture->GetDesc(&desc);
    }
    else if (_pCopyBuffer)
    {
        _pCopyBuffer->GetDesc(&desc);
    }
    else
    {
        return false;
    }

    //为填充帧图像创建一个新的暂存缓冲区
    if (_pCopyBuffer == nullptr) {
        D3D11_TEXTURE2D_DESC CopyBufferDesc;
        CopyBufferDesc.Width = desc.Width;
        CopyBufferDesc.Height = desc.Height;
        CopyBufferDesc.MipLevels = 1;
        CopyBufferDesc.ArraySize = 1;
        CopyBufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        CopyBufferDesc.SampleDesc.Count = 1;
        CopyBufferDesc.SampleDesc.Quality = 0;
        CopyBufferDesc.Usage = D3D11_USAGE_STAGING;
        CopyBufferDesc.BindFlags = 0;
        CopyBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        CopyBufferDesc.MiscFlags = 0;

        hr = _pDX11Dev->CreateTexture2D(&CopyBufferDesc, nullptr, &_pCopyBuffer);
        if (FAILED(hr)) {
            return false;
        }
    }

    if (_pDX11Texture)
    {
        // 将下一个暂存缓冲区复制到新的暂存缓冲区
        _pDX11DevCtx->CopyResource(_pCopyBuffer, _pDX11Texture);
    }

    IDXGISurface *CopySurface = nullptr;
    // 为映射位创建暂存缓冲区
    hr = _pCopyBuffer->QueryInterface(__uuidof(IDXGISurface), (void **)&CopySurface);
    if (FAILED(hr)) {
        return false;
    }

    DXGI_MAPPED_RECT MappedSurface;
    // 将位复制到用户空间
    hr = CopySurface->Map(&MappedSurface, DXGI_MAP_READ);

    //这里根据业务需要处理DXGI_MAPPED_RECT


    CopySurface->Unmap();
    CopySurface->Release();
    CopySurface = nullptr;

    // 释放帧
    if (_pDXGIOutputDup)
    {
        _pDXGIOutputDup->ReleaseFrame();
    }

    qDebug() <<  "dxgi capture start:" <<  QDateTime::currentMSecsSinceEpoch();

    return true;
}


PBITMAPINFO gm::ScreenMgr::CaptureByGdi(HWND hwnd) {
    qDebug() <<  "gdi capture start:" <<  QDateTime::currentMSecsSinceEpoch();
    HDC hdcSrc =  GetWindowDC(hwnd);
    RECT rect; //窗口区域举行
    GetWindowRect(hwnd,&rect);
    int width = 30;
    int height = 25;

    // 创建一个物理设备的内存拷贝
    HDC hdcDest = CreateCompatibleDC(hdcSrc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc,width,height);
    // 选取位图到新的DC中
    HGDIOBJ hOld =  SelectObject(hdcDest,hBitmap);
    //todo:: 这里截取了指定位置的图片，但是显示的还是窗口的宽高
    BitBlt(hdcDest,0,0,width,height,hdcSrc,15,40,SRCCOPY);
    //转存储
    SelectObject(hdcDest, hOld);
    DeleteDC(hdcDest);
    ReleaseDC(hwnd,hdcSrc);

    // 逻辑处理 hBitmap
    PBITMAPINFO pinfo= CreateBitmapInfoStruct(hBitmap);
    CreateBMPFILE(hBitmap);


    DeleteObject(hBitmap);
    qDebug() <<  "gdi capture ended:" <<  QDateTime::currentMSecsSinceEpoch();
    return pinfo;
}

// 根据HBITMAP 生成PBITMAPINFO 位图信息
PBITMAPINFO gm::ScreenMgr::CreateBitmapInfoStruct(HBITMAP hBmp) {
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // 通过GetObject 获取位图的颜色格式、宽高等信息
    assert(GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp));

    // 判断位图的位数信息
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    // BITMAPINFOHEADER 结构分配内存
    if (cClrBits < 24)
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                                        sizeof(BITMAPINFOHEADER) +
                                        sizeof(RGBQUAD) * (1 << cClrBits));
    // 24/32bitmap 就没有调色板信息了
    else
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,
                                        sizeof(BITMAPINFOHEADER));

    // 初始化 BITMAPINFO 各字段
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

    if (cClrBits < 24) pbmi->bmiHeader.biClrUsed = (1<<cClrBits);

    // 如果位图么有压缩，要设置BI_RGB标志
    pbmi->bmiHeader.biCompression = BI_RGB;

    /*
     * 计算调色板数组大小，并存储到biSizeImage中。如果位图没有使用RLE压缩，其宽度必须DWORD对齐
     *  每一行的数据必须以一个long型的为0的整数作为边界，也即每行必须插入一个为0的long, 这就是为什么最后要 & ~31
     *  /8 就是为了 Dword对齐
     * */
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8 *  pbmi->bmiHeader.biHeight;
    // 设置biClrImportant为0，指示所有设备颜色都狠重要
    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi;
}

gm::BMPFILE gm::ScreenMgr::CreateBMPFILE(HBITMAP hBMP) {
    BMPFILE file;
    PBITMAPINFO pbi = CreateBitmapInfoStruct(hBMP);     // 从HBITMAP指针处获取位图信息
    PBITMAPINFOHEADER pbih = (PBITMAPINFOHEADER) pbi;
    HDC hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));

    // 位图数据
    LPBYTE lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
    GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,DIB_RGB_COLORS);

    //要通过PBITMAPINFO 补充完全文件头信息
    file.hdr.bfType= 0x4d42;     //固定值 0x42 = "B" 0x4d = "M"
    //计算整个文件大小
    file.hdr.bfSize =  sizeof(BITMAPFILEHEADER)                   // bmp文件头的大小
                        + pbih->biSize                            // 位图信息头的大小
                        + pbih->biClrUsed * sizeof (RGBQUAD)      // 调色板大小，位图中的实际使用彩色表中的颜色索引数
                        + pbih->biSizeImage;                      // 图像的大小

    file.hdr.bfReserved1 = 0;
    file.hdr.bfReserved2 = 0;
    // 计算位图数据的偏移
    file.hdr.bfOffBits = sizeof(BITMAPFILEHEADER)
                         + pbih->biSize
                         + pbih->biClrUsed * sizeof (RGBQUAD);


    // 测试写入文件
    HANDLE hf = CreateFile(L"./wote.bmp",
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    DWORD dwTmp;

    // 写入文件信息
    WriteFile(hf,&file.hdr,sizeof(BITMAPFILEHEADER ),&dwTmp,NULL);
    // 写入位图信息
    WriteFile(hf,pbih,sizeof(BITMAPINFOHEADER)+ pbih->biClrUsed * sizeof(RGBQUAD),&dwTmp,NULL);
    // 写入位图数据
    WriteFile(hf,lpBits,pbih->biSizeImage,&dwTmp,NULL);

    CloseHandle(hf);
    GlobalFree(lpBits);
    return file;
}