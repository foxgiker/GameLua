/*
    Created by c0518535x on 2023/7/14.
*/

#include <QApplication>
#include "ui/AppMain.h"

//#include "lua.hpp"
//
//
//#include <leptonica/allheaders.h>
//#include <opencv2/opencv.hpp>
//#include <tesseract/baseapi.h>

#include "WindowMgr.h"


//lua_State * LUA;
//
// int luaAdd(int x, int y){
//    int sum;
//    lua_getglobal(LUA,"add");
//    lua_pushnumber(LUA,x);
//    lua_pushnumber(LUA,y);
//    lua_call(LUA,2,1);  // 2个参数一个 返回值
//    sum = (int) lua_tonumber(LUA,-1); // 获取结果
//    lua_pop(LUA,1); // 清除返回值，弹出栈
//    return sum;
//}
//
//
//void testLua(){
//    // Qt调用lua脚本
//    LUA = luaL_newstate(); // 新建lua解释器
//    luaL_openlibs(LUA); //载入lua基础库
//    luaL_dofile(LUA, "../../scripts/lua/ui.lua"); // 执行lua脚本
//
//    int sum = luaAdd(10, 20);
//    qDebug() << "sum: " << sum;
//    lua_close(LUA);
// }
//
//
//void testTesseract(){
//    char *outText;
//
//    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
//    // 使用指定的语言初始化 tesseract-ocr
//    if (api->Init("../../data", "eng")) // you can set tessdata path and language here
//    {
//        fprintf(stderr, "Could not initialize tesseract.\n");
//        exit(1);
//    }
//
//    // Open input image with leptonica library
//    Pix *image = pixRead("../../scripts/eye.tif");
//    api->SetImage(image);
//    // Get OCR result
//    outText = api->GetUTF8Text();
//    printf("---- OCR output:---- \n%s", outText);
//
//    // Destroy used object and release memory
//    api->End();
//    delete[] outText;
//    pixDestroy(&image);
// }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppMain app;
    app.show();
    return a.exec();
}
