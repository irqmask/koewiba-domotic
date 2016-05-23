# KWB Test
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

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbtest_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

set (kwbtest_SRCS
    kwbtest_main.c
    ../shared/ioloop.c
	../shared/log.c
    ../shared/message_socket.c
)

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbtest
        ${kwbtest_SRCS} )
    install ( TARGETS kwbtest
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbtest
        ${kwbtest_SRCS} )
    install ( TARGETS kwbtest
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbtest libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbtest pthread)
    target_link_libraries (kwbtest safec-1.0)
endif (WIN32)

add_dependencies (kwbtest libsystem)

