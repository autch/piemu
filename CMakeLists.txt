cmake_minimum_required(VERSION 3.0)
project(piemu)

option(USE_PROF "Use Google Profiler" OFF)

find_package(PkgConfig)
find_package(SDL2 REQUIRED)
#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

if(MSVC)
add_compile_options(
    /Wall
    /utf-8
)
else()
add_compile_options(
    -g
    -O3
    -funroll-loops
    -Wall
)
endif()

pkg_check_modules(FUSE fuse)
if(FUSE_FOUND)
    include_directories(${FUSE_INCLUDE_DIRS})
    link_directories(${FUSE_LIBRARY_DIRS})
endif()

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(c33core
    core/class0.c
    core/class1.c
    core/class2.c
    core/class3.c
    core/class4.c
    core/class5.c
    core/class6.c
    core/ext.c
    core/common.c

    core/classes.h
    include/core.h
    include/c33209e.h
    include/c33types.h
)

add_executable(piemu
    bcu.c
    emu.c
    fram.c
    lcdc.c
    piemu.c
    usbc.c
    core.c
    flash.c
    iomem.c
    main.c
    sram.c

    app.h
    pfi.h

    include/bcu.h
    include/c33types.h
    include/c33209e.h
    include/core.h
    include/emu.h
    include/flash.h
    include/fram.h
    include/iomap.h
    include/iomem.h
    include/lcdc.h
    include/piece_types.h
    include/piemu.h
    include/sram.h
    include/usbc.h
    include/vector.h
)
target_link_libraries(piemu
    ${SDL2_LIBRARIES}
    c33core
#    SDLmain
)
if (USE_PROF)
    target_compile_definitions(piemu
        PRIVATE
        USE_PROF
    )
    target_link_libraries (piemu profiler)
endif (USE_PROF)

add_subdirectory(./tools)
#add_subdirectory(./tools-old)
