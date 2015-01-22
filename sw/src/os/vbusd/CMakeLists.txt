# vbusd
cmake_minimum_required (VERSION 2.6)

include_directories (
    ../../include
    ../../shared
    ../include
    ../libsystem
    ../shared
    ${CMAKE_CURRENT_BINARY_DIR}
)

# system dependant include directories
# ---------------------------------------------------------
if (WIN32)

else (WIN32)
    include_directories (
        ../3rd_party/libsafec/include
    )
endif (WIN32)

link_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/vbusd_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

set (vbusd_SRCS
    vbusd_main.c
    ../shared/ioloop.c
)

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( vbusd WIN32
        ${vbusd_SRCS} )
    install ( TARGETS vbusd
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( vbusd
        ${vbusd_SRCS} )
    install ( TARGETS vbusd
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (vbusd libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (vbusd safec-1.0)
endif (WIN32)

add_dependencies (vbusd libsystem)
