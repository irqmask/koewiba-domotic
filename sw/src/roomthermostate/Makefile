MCU = atmega324p
FORMAT = ihex
TARGET = roomthermostate_atmega324p

ROOTDIR = $(subst \,/,$(shell pwd))/../..
include ../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/bus_phy.c \
      shared/bus_transport.c \
      shared/clock.c \
      shared/crc16.c \
      shared/disp_st7565.c \
      shared/gdisplay.c \
      shared/gdisplay_font_x8.c \
      shared/gdisplay_font_x16.c \
      shared/gdisplay_symbols_x16.c \
      shared/gdisplay_symbols_x24.c \
      shared/register.c \
      shared/zagwire.c \
      roomthermostate/appregister.c \
      roomthermostate/main_roomthermostate.c

# Place -I options here
INCDIRS = -I. \
          -I../include \
          -I../shared


LOCAL_CFLAGS = -DF_CPU=7372800 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../buildscripts/lib/lib_build_avr.mk
include ../../buildscripts/lib/lib_program_avr.mk
