

add_library(LuaEngine INTERFACE  LuaEngine.cpp)
target_include_directories(LuaEngine INTERFACE .)

# 将lua脚本所在的文件夹复制到输出目录
add_custom_target(LuaScript COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/script" "${CMAKE_BINARY_DIR}/scripts/lua")