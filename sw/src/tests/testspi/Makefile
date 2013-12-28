MCU = atmega324p
FORMAT = ihex
TARGET = testspi_atmega324p

ROOTDIR = $(subst \,/,$(shell pwd))/../../..
include ../../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/eeprom_spi.c \
      shared/spi.c \
      tests/testspi/main_spitest.c

# Place -I options here
INCDIRS = -I. \
          -I../../include \
          -I../../shared


LOCAL_CFLAGS = -DF_CPU=7372800
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../../buildscripts/lib/lib_build_avr.mk
include ../../../buildscripts/lib/lib_program_avr.mk
