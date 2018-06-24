cmake_minimum_required(VERSION 3.0)
project(piemu)

find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++17")

add_compile_options(
    -g
    -Wall
)

pkg_check_modules(FUSE fuse)
if(FUSE_FOUND)
    include_directories(${FUSE_INCLUDE_DIRS})
    link_directories(${FUSE_LIBRARY_DIRS})
endif()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(c33core++
    core/c33core.cpp
    core/c33core.h
    core/c33inst.h
    core/c33core_common.cpp
    core/c33core_ext.cpp
    core/c33core_impl.h

    core/c33core_c0.cpp
    core/c33core_c1.cpp
    core/c33core_c2.cpp
    core/c33core_c3.cpp
    core/c33core_c4.cpp
    core/c33core_c5.cpp
    core/c33core_c6.cpp
)

add_library(c33core
    core/class0.c
    core/class1.c
    core/class2.c
    core/class3.c
    core/class4.c
    core/class5.c
    core/class6.c
    core/classes.h
    core/common.c
    core/ext.c
    core/c33inst.h
    core/core.c
    core/core.h
)

add_executable(piemu
    core/bcu.c
    core/bcu.h
    emu.c
    piemu.c
    main.c
    pfi.h
    app.h

    devices/fram.c
    devices/lcdc.c
    devices/usbc.c
    devices/flash.c
    devices/iomem.c
    devices/sram.c

    devices/flash.h
    devices/fram.h
    devices/iomap.h
    devices/iomem.h
    devices/lcdc.h
    devices/sram.h
    devices/usbc.h
    include/c33types.h
    include/emu.h
    include/piece_types.h
    include/piemu.h
    include/vector.h
)
target_link_libraries(piemu
    ${SDL2_LIBRARIES}
    c33core
    #    SDLmain
)

add_executable(piemu++
    core/c33bcu.cpp

    core/c33bcu.h
    include/c33209e.h
    devices/pme_usbc.h
    devices/pme_fram.h
    devices/pme_sram.h

    devices/pme_flash.h
    devices/pme_flash.cpp
    devices/pme_iomem.cpp
    devices/pme_iomem.h
)

target_link_libraries(piemu++
    ${SDL2_LIBRARIES}
    c33core++
#    SDLmain
)

add_subdirectory(./tools)
#add_subdirectory(./tools-old)
