#TOOLCHAINBASE=/home/freetz/freetz-trunk/toolchain/build/mipsel_gcc-4.5.3_uClibc-0.9.29/mipsel-linux-uclibc
TOOLCHAINBASE=
BINPATH=
#$(TOOLCHAINBASE)/bin
TOOLPREFIX=
#/mipsel-linux-

PROJECT=kwbmonitor

SRC_FILES += kwbmonitor.c
SRC_FILES += PortableSerialLib.c

INCLUDES += -I./

LIBS += -lpthread
# LIBS += -lusb

#OPTFLAGS = -Os -ffunction-sections -Wl,--gc-sections
OPTFLAGS = -Os -ffunction-sections -Wl,--gc-sections

###############################################################
#####
##### Compiler, Linker and Tools
#####
###############################################################

CC=$(BINPATH)$(TOOLPREFIX)gcc
CPP=$(BINPATH)$(TOOLPREFIX)g++
AS=$(BINPATH)$(TOOLPREFIX)as
LD=$(BINPATH)$(TOOLPREFIX)gcc
OC=$(BINPATH)$(TOOLPREFIX)objcopy
OD=$(BINPATH)$(TOOLPREFIX)objdump


#CFLAGS=-pipe -Wall  $(OPTFLAGS) -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64
CFLAGS=-pipe -Wall $(OPTFLAGS)

all:
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROJECT) $(SRC_FILES) $(LIBS)


clean:
	$(RM) -v $(PROJECT) *.o *.elf *.bin *.hex *~

### EOF
