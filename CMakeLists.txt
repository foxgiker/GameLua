cmake_minimum_required(VERSION 3.5)

PROJECT(GameLua VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 配置本项目需要的环境变量
set(ENV{QT_DIR}  D:/Qt/6.5.1/msvc2019_64/lib/cmake)

# 检测配置环境变量
if(DEFINED ENV{QT_DIR})
    message("QT_DIR: $ENV{QT_DIR}")
else()
    message("QT_DIR 安装目录没有配置,该路径要指向安装路径下的lib/cmake文件夹")
endif ()

add_subdirectory(deps)
add_subdirectory(src)