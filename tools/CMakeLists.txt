
add_executable(pfar
    pfar.c
    pffs.c
    pfi.c
)

add_executable(mkpfi
    mkpfi.c
    pffs.c
    pfi.c
)

add_executable(fusepfi
    fusepfi.c
    pfi.c
    pffs.c
)
target_compile_options(fusepfi
    PRIVATE
    ${FUSE_CFLAGS}
)
target_link_libraries(fusepfi
    ${FUSE_LIBRARIES}
)