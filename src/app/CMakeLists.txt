


# 必须要设置这个开关，让cmake自动处理QT中的宏
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


add_executable(GameLua main.cpp AppWindow.cpp AppWindow.h)

target_link_libraries(GameLua PRIVATE  Qt6 LuaBridge Lua54 opencv_48 leptonica_184 tesseract_532)


# 指定生成目录，方便后期调试
set_target_properties(GameLua
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

