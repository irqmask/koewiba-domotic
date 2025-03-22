MCU = atmega328p
FORMAT = ihex
TARGET = roomthermostat_atmega328p

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
      shared/disp_sh1106.c \
      shared/eeprom_spi.c \
      shared/input.c \
      shared/gdisplay.c \
      shared/gdisplay_font_x8.c \
      shared/gdisplay_font_x16.c \
      shared/gdisplay_symbols_x24.c \
      shared/led_debug.c \
      shared/main_avr.c \
      shared/messaging.c \
      shared/register.c \
      shared/sleepmode.c \
      shared/timer.c \
      shared/zagwire.c \
      basic_app_atmega328p/application.c \
      basic_app_atmega328p/appregister.c \
      basic_app_atmega328p/appversion.c

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
