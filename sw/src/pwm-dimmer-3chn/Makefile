MCU = atmega88
FORMAT = ihex
TARGET = pwm-dimmer-3chn_atmega88

ROOTDIR = $(subst \,/,$(shell pwd))/../..
include ../../buildscripts/lib/lib_outputdirs.mk

# C and assembler source files used in current project
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC = shared/bus_phy.c \
      shared/bus_transport.c \
      shared/clock.c \
      shared/crc16.c \
      shared/eeprom_spi.c \
      shared/pwm.c \
      shared/queue.c \
      shared/register.c \
      pwm-dimmer-3chn/appregister.c \
      pwm-dimmer-3chn/main_pwm-dimmer-3chn.c

# Place -I options here
INCDIRS = -I. \
          -I../include \
          -I../shared

#-DF_CPU=7372800
LOCAL_CFLAGS = -DF_CPU=16000000 -DHAS_PCBCONFIG_H -DHAS_APPCONFIG_H
LOCAL_ASFLAGS =
LOCAL_LDFLAGS =

include ../../buildscripts/lib/lib_build_avr.mk
include ../../buildscripts/lib/lib_program_avr.mk
