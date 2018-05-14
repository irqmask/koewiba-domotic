MCU = atmega328p
FORMAT = ihex
TARGET = blindcontrol_5+4_atmega328p

ROOTDIR = $(subst \,/,$(shell pwd))/..
include ../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/block_message.c \
      shared/bus_phy.c \
      shared/datetime.c \
      shared/eeprom_spi.c \
      shared/led_debug.c \
      shared/main_avr.c \
      shared/register.c \
      shared/sleepmode.c \
      shared/timer.c \
      blind-control/blindctrl.c \
      blind-control/motor.c \
      blind-control_5+4_atmega328p/application.c \
      blind-control_5+4_atmega328p/appregister.c \
      blind-control_5+4_atmega328p/appversion.c \


# Place -I options here
INCDIRS = -I. \
          -I../blind-control \
          -I../shared \
          -I../../include \
          -I../../shared \



LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS = -T ../include/avr5_kwb.x

include ../buildscripts/lib/lib_build_avr.mk
include ../buildscripts/lib/lib_program_avr.mk
