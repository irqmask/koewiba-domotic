MCU = atmega328p
FORMAT = ihex
TARGET = test_motor_switch_5_atmega328p

ROOTDIR = $(subst \,/,$(shell pwd))/../..
include ../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = ../shared/bus_transport.c \
      ../shared/crc16.c \
      ../shared/queue.c \
      shared/block_message.c \
      shared/bus_phy.c \
      shared/eeprom_spi.c \
      shared/led_debug.c \
      shared/main_avr.c \
      shared/register.c \
      shared/sleepmode.c \
      shared/sn74595.c \
      shared/timer.c \
      blind-control/motor_relay_serial.c \
      tests/test_switch_5_atmega328p/application.c \
      tests/test_switch_5_atmega328p/appregister.c \
      tests/test_switch_5_atmega328p/appversion.c \
      tests/test_switch_5_atmega328p/test_io.c \
      tests/shared/test_eeprom.c

# Place -I options here
INCDIRS = -I. \
          -I../../shared \
          -I../../../include \
          -I../../../shared


LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS = -T ../../include/avr5_kwb.x

include ../../buildscripts/lib/lib_build_avr.mk
include ../../buildscripts/lib/lib_program_avr.mk
