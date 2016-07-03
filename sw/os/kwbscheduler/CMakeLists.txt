# KWB Scheduler
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

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbscheduler_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

# header and source files
# ---------------------------------------------------------
set (include_SRCS
    ../../include/cmddef_common.h
    ../../include/moddef_common.h
    ../../include/prjconf.h
    ../../include/prjtypes.h
)

set (shared_SRCS
    ../../shared/bus.h
    ../../shared/clock.h
    ../../shared/bus_schedul.c
    ../../shared/bus_transport.c
    ../../shared/crc16.c
    ../../shared/queue.c
)

set (os_include_SRCS
    ../include/error_codes.h
)

set (os_shared_SRCS
    ../shared/log.h
    ../shared/message.h

    ../shared/bus_phy_os.c
    ../shared/clock_os.c
    ../shared/ioloop.c
    ../shared/log.c
    ../shared/message_bus.c
    ../shared/vos.c
)

set (kwbscheduler_SRCS
     kwbscheduler_main.c
     )

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${kwbscheduler_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbscheduler ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${kwbscheduler_SRCS} )
    install ( TARGETS kwbscheduler
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbscheduler ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${kwbscheduler_SRCS} )
    install ( TARGETS kwbscheduler
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbscheduler libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbscheduler safec-1.0)
endif (WIN32)

add_dependencies (kwbscheduler libsystem)
