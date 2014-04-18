# KWB Unittests
cmake_minimum_required (VERSION 2.6)
INCLUDE_DIRECTORIES (
    .
    ../src/include
    ../src/shared
    ${CMAKE_CURRENT_BINARY_DIR}
)

LINK_DIRECTORIES (
    ${CMAKE_CURRENT_BINARY_DIR}
)

FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwb_unittests_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")


SET(kwb_unittests_SRCS
    ../src/shared/bus_schedul.c
    ../src/shared/bus_transport.c
    ../src/shared/pwm.c
    testbussched.c
    testpwm.c
    register_tests.c
    main.c
)

ADD_DEFINITIONS(-DHAS_APPCONFIG_H)
ADD_DEFINITIONS(-DHAS_PCBCONFIG_H)


IF (WIN32)
    ADD_EXECUTABLE ( kwb_unittests WIN32
        ${kwb_unittests_SRCS} )
    INSTALL ( TARGETS kwb_unittests
              RUNTIME DESTINATION . )
ELSE (WIN32)
    ADD_EXECUTABLE ( kwb_unittests
        ${kwb_unittests_SRCS} )
    INSTALL ( TARGETS kwb_unittests
              RUNTIME DESTINATION bin )
ENDIF (WIN32)

TARGET_LINK_LIBRARIES(kwb_unittests cunit)
