cmake_minimum_required(VERSION 3.12)

# PROJECT(GameLua VERSION 1.0.0 LANGUAGES CXX)
PROJECT(GameLua CXX C )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

# 配置本项目需要的环境变量
set(ENV{QT_DIR}  D:/Qt/6.5.1/msvc2019_64/lib/cmake)
set(ENV{leptonica_DIR} D:/cpp-repository/leptonica_x64/lib/cmake/leptonica)
set(ENV{tesseract_DIR} D:/cpp-repository/tesseract_x64-windows/lib/cmake/tesseract)


# 检测配置环境变量

if(DEFINED ENV{qt_DIR})
    message("qt_DIR: $ENV{qt_DIR}")
    list(APPEND CMAKE_PREFIX_PATH $ENV{qt_DIR})
else()
    message(FATAL_ERROR "qt_DIR 安装目录没有配置,该路径要指向安装路径下的lib/cmake文件夹")
endif ()

if(DEFINED ENV{leptonica_DIR})
    message("leptonica_DIR: $ENV{leptonica_DIR}")
    list(APPEND CMAKE_PREFIX_PATH $ENV{leptonica_DIR})
else()
    message(FATAL_ERROR "leptonica_DIR 安装目录没有配置")
endif ()


if(DEFINED ENV{tesseract_DIR})
    message("tesseract_DIR: $ENV{tesseract_DIR}")
    list(APPEND CMAKE_PREFIX_PATH $ENV{tesseract_DIR})
else ()
    message("tesseract_DIR 环境变量没有配置")
endif ()




add_subdirectory(deps)
add_subdirectory(src)
