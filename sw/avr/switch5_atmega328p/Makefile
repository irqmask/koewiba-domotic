MCU = atmega328p
FORMAT = ihex
TARGET = switch5_atmega328p

ROOTDIR = $(subst \,/,$(shell pwd))/..
include ../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/alarmclock.c \
      shared/block_message.c \
      shared/bus_phy.c \
      shared/datetime.c \
      shared/eeprom_spi.c \
      shared/input.c \
      shared/led_debug.c \
      shared/main_avr.c \
      shared/register.c \
      shared/sleepmode.c \
      shared/timer.c \
      switch5_atmega328p/application.c \
      switch5_atmega328p/appregister.c \
      switch5_atmega328p/appversion.c \
      switch5_atmega328p/output.c

# Place -I options here
INCDIRS = -I. \
          -I../include \
          -I../shared \
          -I../../include \
          -I../../shared


LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS = -T ../include/avr5_kwb.x

include ../buildscripts/lib/lib_build_avr.mk
include ../buildscripts/lib/lib_program_avr.mk
