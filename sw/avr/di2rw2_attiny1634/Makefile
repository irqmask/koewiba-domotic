MCU = attiny1634
FORMAT = ihex
TARGET = di2rw2_attiny1634

ROOTDIR = $(subst \,/,$(shell pwd))/..
include ../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/block_message.c \
      shared/bus_phy.c \
      shared/eeprom_spi.c \
      shared/input.c \
      shared/led_debug.c \
      shared/main_avr.c \
      shared/register.c \
      shared/sleepmode.c \
      shared/timer.c \
      di2rw2_attiny1634/application.c \
      di2rw2_attiny1634/appregister.c \
      di2rw2_attiny1634/appversion.c \
      di2rw2_attiny1634/relays_keys.c

# Place -I options here
INCDIRS = -I. \
          -I../shared \
          -I../../include \
          -I../../shared

LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS = -T ../include/avr35_kwb.x

include ../buildscripts/lib/lib_build_avr.mk
include ../buildscripts/lib/lib_program_avr.mk
