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

set (kwbscheduler_SRCS
     kwbscheduler_main.c
     ../shared/bus_phy_os.c
     ../shared/clock_os.c
     ../shared/ioloop.c
     ../shared/message_bus.c
     ../shared/vos.c
     ../../shared/bus_schedul.c
     ../../shared/bus_transport.c
     ../../shared/crc16.c
     )

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbscheduler
        ${kwbscheduler_SRCS} )
    install ( TARGETS kwbscheduler
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbscheduler
        ${kwbscheduler_SRCS} )
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
#add_definitions (-DBUS_SCHEDULER)