# CMake build script for libzerotiercore.a

cmake_minimum_required (VERSION 2.8)
project (zerotiercore)

set (PROJ_DIR ${PROJECT_SOURCE_DIR})
set (ZT_DEFS -std=c++11)

file(GLOB core_src_glob ${PROJ_DIR}/node/*.cpp)
add_library(zerotiercore STATIC ${core_src_glob})

target_compile_options(zerotiercore PRIVATE ${ZT_DEFS})
# 添加头文件搜索路径
include_directories(.)
include_directories(attic/)
include_directories(ext/)

# 添加可执行文件编译目标
add_executable(mkworld
        node/C25519.cpp
        node/Salsa20.cpp
        node/SHA512.cpp
        node/Identity.cpp
        node/Utils.cpp
        node/InetAddress.cpp
        osdep/OSUtils.cpp
        attic/world/mkworld.cpp
)

add_executable(read_planet
        node/C25519.cpp
        node/Salsa20.cpp
        node/SHA512.cpp
        node/Identity.cpp
        node/Utils.cpp
        node/InetAddress.cpp
        osdep/OSUtils.cpp
        attic/world/read_planet.cpp
)

# 链接数学库
target_link_libraries(mkworld m)
target_link_libraries(read_planet m)
