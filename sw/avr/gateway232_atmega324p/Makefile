MCU = atmega324p
FORMAT = ihex
TARGET = gateway232_atmega324p

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
      shared/register.c \
      shared/sleepmode.c \
      shared/timer.c \
      gateway232_atmega324p/appregister.c \
      gateway232_atmega324p/appversion.c \
      gateway232_atmega324p/bus_gateway.c \
      gateway232_atmega324p/main_gateway.c \
      gateway232_atmega324p/serialcomm.c \

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
