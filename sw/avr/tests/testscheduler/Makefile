MCU = atmega88
FORMAT = ihex
TARGET = testscheduler_atmega88

ROOTDIR = $(subst \,/,$(shell pwd))/../..

include ../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_scheduler.c \
      ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/bus_phy.c \
      shared/clock.c \
      shared/led_debug.c \
      shared/sleepmode.c \
      tests/testscheduler/main_testscheduler.c

# Place -I options here
INCDIRS = -I. \
          -I../../include \
          -I../../shared \
          -I../../../include \
          -I../../../shared \


LOCAL_CFLAGS = -DF_CPU=16000000 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../buildscripts/lib/lib_build_avr.mk
include ../../buildscripts/lib/lib_program_avr.mk
