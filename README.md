# GameLua

#### 项目目标
基于luabridge ，实现qt与lua相互调用，从而可以通过lua脚本来构建界面，也可以通过lua来实现业务逻辑。

#### 目录结构
```html
GameLua
    |-deps
        |-Qt6
        |-lua-5.4.2_Win64_vc16_lib
        |-LuaBridge-2.8
    |-src
        |-app
        |-common
    CMakeLists.txt
```
对目录结构的说明：
- deps目录用来存放该项目的外部的依赖，便于项目的分发，就不使用vcpkg了
- src 下用于存放项目的源码， common目录用于存放项目公用的一些源码，app存放主程序的源码，其他的动态库按照这个层级依次增加

通过这个项目，对CMake的设计哲学了解就比较清楚了：

- Cmake中有一个核心概念叫 Target ,一个Target就是一个构建规则，Target主要有三种： 可执行的目标、库类目标、自定义目标
- Cmake中通过 add_subdirectory 来一层层的递归管理项目的构建

本文就是基于以上两条规则组织构建的,其他的编译选项等细节可以在后面接触到的时候再继续学习


#### Cmake添加qt依赖

在deps下面的Qt6目录下 CmakeLists.txt内容如下：

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#设置了 find_package 查找Qt6的路径
set(CMAKE_PREFIX_PATH $ENV{QT_DIR})

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# 在整个项目范围内设置项目为典型的Qt应用， 主要用来处理一些额外的宏
qt_standard_project_setup()

add_library(Qt6 INTERFACE)
target_link_libraries(Qt6 INTERFACE Qt6::Core Qt6::Widgets)
```
这里定义了一个Target 名为 Qt6, 这个目标是通过add_library 定义的，而且她的范围被定义为 INTERFACE， 也即只
作为一个依赖库供其他模块调用，而不是要正真的构建这个库。

在app模块中，只要使用`target_link_library`引入Qt6目标即可,注意一定要开启CMAKE_AUTOMOC选项，否则qt中的一些宏就无法正常展开编译。
```cmake
# 必须要设置这个开关，让cmake自动处理QT中的宏
set(CMAKE_AUTOMOC ON)
add_executable(GameLua main.cpp AppWindow.cpp AppWindow.h)
target_link_libraries(GameLua PRIVATE Qt6)
```

#### 整合Lua、LuaBridge
**Lua**

将Lua编译好的包放到 deps目录下，然后添加一个 CMakeLists.txt，内容如下：
```cmake
add_library(Lua54 INTERFACE lua54.lib)
target_include_directories(Lua54 INTERFACE include)
```

这里定义了一个库目标，同时指定了这个目标包含的头文件目录。 在app模块下直接 连接自定义的这个Lua54即可。

**LuaBridge**
直接吧git中下载luaBridge 放到deps目录下即可， 由于Luabridge中已经有CMakeList.txt文件了，就不用再建了

app模块引入 lua和 luabridge
```cmake
target_link_libraries(GameLua PRIVATE Lua54)
target_link_libraries(GameLua PRIVATE LuaBridge)
```
注意： 这里只需要使用 target_link_libraries来链接目标即可，不需要再使用 target_include_directories来添加头文件，
因为在定义目标的时候 已经包含了。 另外需要注意的是链接的目标名 跟实际的文件夹的名字无关。



