cmake_minimum_required(VERSION 3.16)

PROJECT(GameLua VERSION 1.0.0 LANGUAGES CXX C )
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)


set(ENV{qt_DIR} D:/Qt/6.5.1/msvc2019_64/lib/cmake)
set(ENV{OpenCV_DIR} D:/cpp-repository/opencv-4.8.0/build)
set(ENV{WinIo_DIR} ${CMAKE_SOURCE_DIR}/deps/winio)


add_definitions(-DLUA_USE_DLOPEN)


add_subdirectory(deps/luabridge3)
add_subdirectory(src)





