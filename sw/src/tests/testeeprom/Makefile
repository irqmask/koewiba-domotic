MCU = atmega324p
FORMAT = ihex
TARGET = testeeprom_atmega324p

ROOTDIR = $(subst \,/,$(shell pwd))/../../..
include ../../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/eeprom_spi.c \
      shared/uart.c \
      shared/queue.S \
      tests/testeeprom/main_testeeprom.c

# Place -I options here
INCDIRS = -I. \
          -I../../include \
          -I../../shared


LOCAL_CFLAGS = -DF_CPU=7372800
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../../buildscripts/lib/lib_build_avr.mk
include ../../../buildscripts/lib/lib_program_avr.mk
