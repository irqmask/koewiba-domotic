MCU = attiny1634
FORMAT = ihex
TARGET = bootloader_attiny1634

ROOTDIR = $(subst \,/,$(shell pwd))/..
include ../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/eeprom_spi.c \
      bootloader/main_bootloader.c

# Place -I options here
INCDIRS = -I. \
          -I../include \
          -I../shared \
          -I../../include 

LOCAL_CFLAGS = -DF_CPU=7372800
LOCAL_ASFLAGS =

# Modified linker flags for bootloader!
#  for ATMEGA32 / 324: -Ttext=0x7800
# Addressrange in words:
# $0000 - $3BFF
# $3C00 - $3FFF
# so the beginning of the bootloader is at byteaddress 2*$3C00 = $7800
# size of the bootloader is 1024 words = 2048 bytes
LOCAL_LDFLAGS = -Ttext=0x7800

include ../buildscripts/lib/lib_build_avr.mk
include ../buildscripts/lib/lib_program_avr.mk
