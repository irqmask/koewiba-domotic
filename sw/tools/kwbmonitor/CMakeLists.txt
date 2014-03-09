# KWB Monitor
cmake_minimum_required (VERSION 2.6)
INCLUDE_DIRECTORIES (
    .
    ../../src/include
    ../../src/shared
    ${CMAKE_CURRENT_BINARY_DIR}
)
# system dependant include directories
# ---------------------------------------------------------
IF (WIN32)

ELSE (WIN32)
INCLUDE_DIRECTORIES (
    ../../3rd_party/libsafec/include
)
ENDIF (WIN32)

LINK_DIRECTORIES (
    ${CMAKE_CURRENT_BINARY_DIR}
)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwb_monitor_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

SET(kwbmonitor_SRCS
    crc16.c
    kwbmonitor.c
    KWBMonitorDisplay.c
    PortableSerialLib.c
    sysconsole.c
    systime.c
)

# executable
# ---------------------------------------------------------
IF (WIN32)
    ADD_EXECUTABLE ( kwbmonitor WIN32
        ${kwbmonitor_SRCS} PortableSerialLibWindows.c)
    INSTALL ( TARGETS kwbmonitor
              RUNTIME DESTINATION . )
ELSE (WIN32)
    ADD_EXECUTABLE ( kwbmonitor
        ${kwbmonitor_SRCS} PortableSerialLibLinux.c)
    INSTALL ( TARGETS kwbmonitor
              RUNTIME DESTINATION bin )
ENDIF (WIN32)

# Link libraries
# ---------------------------------------------------------
IF (WIN32)

ELSE (WIN32)
TARGET_LINK_LIBRARIES(kwbmonitor pthread)
TARGET_LINK_LIBRARIES(kwbmonitor safec-1.0)
ENDIF (WIN32)



