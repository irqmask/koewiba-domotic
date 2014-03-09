BINPATH=

PROJECT=kwbcommands

SRC_FILES += BusCommunication.c bus_phy_linux.c clock_linux.c KWBCommands.c sysconsole.c systime.c 
SRC_FILES += PortableSerialLib.c
SRC_FILES += ../../src/shared/bus_transport.c
SRC_FILES += ../../src/shared/crc16.c

INCLUDES += -I./ -I../../src/shared -I../../src/include -I../../3rd_party/libsafec/include

LIBS += -lpthread -lsafec-1.0

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


