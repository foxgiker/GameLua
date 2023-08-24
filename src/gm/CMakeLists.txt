
# 必须要设置这个开关，让cmake自动处理QT中的宏
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_library(gm STATIC WindowMgr.cpp KeyMouseMgr.h KeyMouseMgr.cpp KeyMouseMgr.cpp KeyMouseMgr.cpp ScreenMgr.h ScreenMgr.cpp
        OCRHelper.h
        OCRHelper.cpp
        BitMapMem.h
        BitMapMem.cpp
        LuaRunner.h
        LuaRunner.cpp
        CVHelper.h
        CVHelper.h
        CVHelper.cpp
        LuaControl.h
        LuaControl.cpp
        LuaControl.cpp
        Global.h
        Global.h
)



find_package(Qt6 COMPONENTS Core Widgets)

find_package(lua REQUIRED)
message("lua Version: ${lua_VERSION_STRING}")


find_package(Leptonica REQUIRED)
message("Leptonica Version: ${Leptonica_VERSION_STRING}")


find_package(Tesseract REQUIRED)
message("Tesseract Version: ${Tesseract_VERSION_STRING}")

FIND_PACKAGE(OpenCV REQUIRED)
message("OpenCV Version: ${OpenCV_VERSION}")




target_link_libraries(gm PUBLIC
        ${OpenCV_LIBS}
        ${Leptonica_LIBRARIES}
        ${Tesseract_LIBRARIES}
        ${lua_LIBRARIES}
        LuaBridge
        Qt6::Core Qt6::Widgets)

target_include_directories(gm PUBLIC .
        ${lua_INCLUDE_DIRS}
        ${Leptonica_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)



