cmake_minimum_required(VERSION 3.0)
project(piemu)
find_package(PkgConfig REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

add_compile_options(
    -g
    -Wall
)

#pkg_check_modules(SDL2 REQUIRED sdl2)
#include_directories(${SDL2_INCLUDE_DIRS})
#link_directories(${SDL2_LIBRARY_DIRS})

pkg_check_modules(SDL1 REQUIRED sdl)
include_directories(${SDL1_INCLUDE_DIRS})
link_directories(${SDL1_LIBRARY_DIRS})

pkg_check_modules(FUSE REQUIRED fuse)
include_directories(${FUSE_INCLUDE_DIRS})
link_directories(${FUSE_LIBRARY_DIRS})

add_executable(piemu
    bcu.c   emu.c    fram.c   lcdc.c  piemu.c  usbc.c
    core.c  flash.c  iomem.c  main.c  sram.c
)
target_include_directories(piemu
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
target_compile_options(piemu
    PRIVATE
    ${SDL1_CFLAGS})
target_link_libraries(piemu
    ${SDL1_LIBRARIES}
    SDLmain
)

add_subdirectory(./tools)
#add_subdirectory(./tools-old)

