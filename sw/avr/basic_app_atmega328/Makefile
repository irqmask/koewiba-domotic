MCU = atmega328p
FORMAT = ihex
TARGET = basic_app_atmega328

ROOTDIR = $(subst \,/,$(shell pwd))/..
include ../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/block_message.c \
      shared/bus_phy.c \
      shared/clock.c \
      shared/eeprom_spi.c \
      shared/led_debug.c \
      shared/register.c \
      basic_app_atmega328/appregister.c \
      basic_app_atmega328/main_basic_app_atmega328.c

# Place -I options here
INCDIRS = -I. \
          -I../shared \
          -I../../include \
          -I../../shared


LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H -D__AVR_ATmega328__
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../buildscripts/lib/lib_build_avr.mk
include ../buildscripts/lib/lib_program_avr.mk
