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

这里的这个整合过程，简直让人崩溃。但整合完成以后，又觉得特别的简单，总之都是不仔细给引发的。 下面记录一下：

整合Lua 有三种方式： 源码整合 或者 预编译整合 或者通过vcpkg . 三者各有优点

1. 通过vcpkg

**优劣**： vckpkg 管理依赖最方便，但是vcpkg仓库中的lua可能不是最新版本。

通过 `vcpkg install lua:x64-windows` 自动安装 lua库

然后通过
```cmake
find_package(Lua REQUIRED)   # 注意大小写

if(Lua_FOUND)
    link_libraries( ${LUA_LIBRARIES})  # 添加链接库
    include_directories(GameLua ${LUA_INCLUDE_DIR})  # 添加头文件
endif ()
```

2. 通过预编译的lua进行整合

**优劣**：好处是能够使用最新的版本，也能够省去每次编译Lua的成本， 但是最大的坏处是，预编译的文件使用的工具链可能
与开发者使用的工具链不一样，从而导致程序编译时的各种莫名其妙的问题

将Lua编译好的包放到 deps目录下，然后添加一个 CMakeLists.txt，内容如下：
```cmake
add_library(Lua54 INTERFACE lua54.lib)
target_include_directories(Lua54 INTERFACE include)
```

这里定义了一个库目标，同时指定了这个目标包含的头文件目录。 在app模块下直接 连接自定义的这个Lua54即可。
也可以通过 `include_directories` 和 `link_directories` 进行全局引入.


3. 通过源码引入：

**优劣**： 能够控制使用的版本、方便项目分发、能够确保编译的库文件与本项目使用的工具链一致，避免莫名其妙的编译问题。

直接把 Lua 源码复制到 deps/lua-5.4.6/src下， 在上级目录中添加如下camke

```cmake
add_library(Lua54 STATIC
        ./src/lcode.c
        ./src/lbaselib.c
        ./src/lauxlib.c
        ./src/lapi.c
        ./src/loslib.c
        ./src/lopcodes.c
        ./src/lobject.c
        ./src/loadlib.c
        ./src/lmem.c
        ./src/lmathlib.c
        ./src/llex.c
        ./src/liolib.c
        ./src/linit.c
        ./src/lgc.c
        ./src/lfunc.c
        ./src/ldump.c
        ./src/ldo.c
        ./src/ldebug.c
        ./src/ldblib.c
        ./src/lctype.c
        ./src/lcorolib.c
        ./src/lzio.c
        ./src/lvm.c
        ./src/lutf8lib.c
        ./src/lundump.c
        ./src/ltm.c
        ./src/ltablib.c
        ./src/ltable.c
        ./src/lstring.c
        ./src/lstrlib.c
        ./src/lstate.c
        ./src/lparser.c
        )

target_include_directories(Lua54 PUBLIC ./src)
```

在需要依赖的 lua的项目中 只需要添加依赖即可

```cmake
target_link_libraries(GameLua PRIVATE  Qt6 LuaBridge Lua54)
```

整合遇到的问题：

在尝试了上述几种整合方式后，头文件能够正确定位，通过观察 link命令，确保lua库也能够正确的链接。同时也注意到网上提醒的
要用 `extern "c" {}` 来包裹include 语句。 但是每次编译总是会报错：

```
error LNK2019: unresolved external symbol "yoid _-cdecl lua_close
```

经过两天时间的苦苦查找，最终发现问题的所在： 虽然在main.cpp的测试函数中使用了`include lua.hpp`,但是之前不小心在
`AppWindow.h` 中为了测试lua头文件是否正确引入而写的 `include lua.h` 却没有删除！就导致了一致报错

**解释**： 链接时出现未解析的外部符号，一般都是因为库类中的符号没有找到。 在本例中由于 lua是一个 C库， C库的函数编译与Cpp
的函数编译符号存在着差异（Cpp是面向对象语言，允许函数重载，所以Cpp编译后的函数符号除了函数名外还有参数、类型等的hash字符串，
而C编译后的函数符号就只是函数名，所以在Cpp中如果引用C函数，就需要使用 `extern "C" {} 包裹引入的头文件`。一般的库类，如lua.hpp
都会提供一个 hpp的文件，在里面使用extern将头文件处理好），所以链接时就总是报错。


**LuaBridge**

直接吧git中下载luaBridge 放到deps目录下即可， 由于Luabridge中已经有CMakeList.txt文件了，就不用再建了

app模块引入 lua和 luabridge
```cmake
target_link_libraries(GameLua PRIVATE Lua54)
target_link_libraries(GameLua PRIVATE LuaBridge)
```
注意： 这里只需要使用 target_link_libraries来链接目标即可，不需要再使用 target_include_directories来添加头文件，
因为在定义目标的时候 已经包含了。 另外需要注意的是链接的目标名 跟实际的文件夹的名字无关。



