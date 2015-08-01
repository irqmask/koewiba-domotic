MCU = atmega324p
FORMAT = ihex
TARGET = testgateway232_atmega324

ROOTDIR = $(subst \,/,$(shell pwd))/../..
include ../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/bus_phy.c \
      shared/clock.c \
      shared/register.c \
      tests/testgateway232_atmega324/appregister.c \
      tests/testgateway232_atmega324/bus_gateway.c \
      tests/testgateway232_atmega324/main_gateway.c \
      tests/testgateway232_atmega324/serialcomm.c \

# Place -I options here
INCDIRS = -I. \
          -I../../include \
          -I../../shared \
          -I../../../include \
          -I../../../shared


LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H -D__AVR_ATmega324P__
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../buildscripts/lib/lib_build_avr.mk
include ../../buildscripts/lib/lib_program_avr.mk
