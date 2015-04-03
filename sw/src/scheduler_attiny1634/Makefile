MCU = attiny1634
FORMAT = ihex
TARGET = scheduler_attiny1634

ROOTDIR = $(subst \,/,$(shell pwd))/../..

include ../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/bus_phy.c \
      shared/bus_schedul.c \
      shared/bus_transport.c \
      shared/bus_tx_queue.c \
      shared/clock.c \
      shared/crc16.c \
      scheduler_attiny1634/appversion.c \
      scheduler_attiny1634/main_scheduler.c

# Place -I options here
INCDIRS = -I. \
          -I../include \
          -I../shared


LOCAL_CFLAGS = -DF_CPU=7367280 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS = -T ../avr/include/avr35_kwb.x

include ../../buildscripts/lib/lib_build_avr.mk
include ../../buildscripts/lib/lib_program_avr.mk
#-Wl,--section-start=.versioninfo=0x0100