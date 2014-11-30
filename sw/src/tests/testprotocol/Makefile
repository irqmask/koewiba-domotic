MCU = atmega88
FORMAT = ihex
TARGET = testprotocol_atmega88

ROOTDIR = $(subst \,/,$(shell pwd))/../../..
include ../../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/bus_phy.c \
      shared/bus_transport.c \
      shared/bus_tx_queue.c \
      shared/clock.c \
      shared/crc16.c \
      shared/led_debug.c \
      shared/register.c \
      tests/testprotocol/appregister.c \
      tests/testprotocol/main_testprotocol.c

# Place -I options here
INCDIRS = -I. \
          -I../../include \
          -I../../shared


LOCAL_CFLAGS = -DF_CPU=16000000 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H -D__AVR_ATmega88__
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../../buildscripts/lib/lib_build_avr.mk
include ../../../buildscripts/lib/lib_program_avr.mk
