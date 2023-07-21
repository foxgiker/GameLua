set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#设置了 find_package 查找Qt6的路径
#set(CMAKE_PREFIX_PATH $ENV{QT_DIR})

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# 在整个项目范围内设置项目为典型的Qt应用， 主要用来处理一些额外的宏
qt_standard_project_setup()

add_library(Qt6 INTERFACE)
target_link_libraries(Qt6 INTERFACE Qt6::Core Qt6::Widgets)