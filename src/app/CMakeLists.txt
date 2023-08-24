
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(DEST_PATH "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}")




add_executable(GameLua  main.cpp
        ui/Config.h
        ui/AppMain.h
        ui/AppMain.cpp
        ui/QTableWind.h
        ui/QTableWind.cpp
        ui/predefine.h
        ui/predefine.h
        thread/ThreadLuaRunner.h
        thread/ThreadLuaRunner.cpp)
target_link_options(GameLua PRIVATE "/MANIFESTUAC:level='requireAdministrator'" PRIVATE "/MANIFESTUAC:uiAccess='false'")

target_link_libraries(GameLua PRIVATE gm)




# 指定生成目录，方便后期调试
set_target_properties(GameLua
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

# 将lua脚本所在的文件夹复制到输出目录
add_custom_command(TARGET GameLua PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/script" "${DEST_PATH}/lua"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/data" "${DEST_PATH}/data"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/winio/WinIo64.dll" "${DEST_PATH}/WinIo64.dll"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/deps/winio/WinIo64.sys" "${DEST_PATH}/WinIo64.sys"
        )