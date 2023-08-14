
get_filename_component(CURRENT_PATH "${CMAKE_CURRENT_LIST_FILE}" DIRECTORY)

if(EXISTS ${CURRENT_PATH}/include/winio/winio.h AND EXISTS  ${CURRENT_PATH}/lib/${CMAKE_BUILD_TYPE}/WinIo.lib)
    set(WinIo_LIBRARIES ${CURRENT_PATH}/lib/${CMAKE_BUILD_TYPE}/WinIo.lib)
    set(WinIo_INCLUDE_DIRS ${CURRENT_PATH}/include)
else()
    MESSAGE(FATAL_ERROR "WinIo库文件不完整！")
endif ()



