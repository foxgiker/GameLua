
# 必须要设置这个开关，让cmake自动处理QT中的宏
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_library(gm STATIC WindowMgr.cpp WinIoMgr.h WinIoMgr.cpp WinIoMgr.cpp WinIoMgr.cpp ScreenMgr.h ScreenMgr.cpp
        OCRHelper.h
        OCRHelper.cpp
        BitMapMem.h
        BitMapMem.cpp
        LuaRunner.h
        LuaRunner.cpp
        LuaRegister.h
        LuaRegister.cpp
        CVHelper.h
        CVHelper.h
        CVHelper.cpp)



find_package(Qt6 COMPONENTS Core Widgets)

find_package(lua REQUIRED)
message("lua Version: ${lua_VERSION_STRING}")


find_package(Leptonica REQUIRED)
message("Leptonica Version: ${Leptonica_VERSION_STRING}")


find_package(Tesseract REQUIRED)
message("Tesseract Version: ${Tesseract_VERSION_STRING}")

FIND_PACKAGE(OpenCV REQUIRED)
message("OpenCV Version: ${OpenCV_VERSION}")

find_package(WinIo REQUIRED)
message("WinIo include: ${WinIo_INCLUDE_DIRS}")
message("WinIo lib: ${WinIo_LIBRARIES}")



target_link_libraries(gm PUBLIC
        ${WinIo_LIBRARIES}
        ${OpenCV_LIBS}
        ${Leptonica_LIBRARIES}
        ${Tesseract_LIBRARIES}
        ${lua_LIBRARIES}
        LuaBridge
        Qt6::Core Qt6::Widgets)

target_include_directories(gm PUBLIC .
        ${WinIo_INCLUDE_DIRS}
        ${lua_INCLUDE_DIRS}
        ${Leptonica_INCLUDE_DIRS}
        ${Tesseract_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)



