


# 必须要设置这个开关，让cmake自动处理QT中的宏
set(CMAKE_AUTOMOC ON)
add_executable(GameLua main.cpp AppWindow.cpp AppWindow.h)

target_link_libraries(GameLua PRIVATE Qt6)
target_link_libraries(GameLua PRIVATE Lua54)
target_link_libraries(GameLua PRIVATE LuaBridge)