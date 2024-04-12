#############################################################################
# Makefile for building STM8 program
#############################################################################

MAKEFILE      = Makefile

####### Files

TARGET        = heatervalve

SOURCES       = adc.c \
                cmd.c \
                control_temp.c \
                encoder.c \
                lcd.c \
                main.c \
                motor.c \
                remote_tempsense.c \
                timer.c \
                uart.c \

INCPATH       = -I. -I ../include -I/usr/local/share/sdcc/include

####### Output directories

BUILDDIR      = ../build/$(TARGET)
DESTDIR       = ../bin

OBJECTS       = $(SOURCES:.c=.rel)
OBJECTS_W_PATH= $(addprefix $(BUILDDIR)/,$(OBJECTS))

####### Compiler, tools and options

CC            = sdcc
CXX           = sdcc
DEFINES       = -DHAS_APPCONFIG_H
CFLAGS        = -mstm8 --verbose --no-xinit-opt $(DEFINES)
CXXFLAGS      = -mstm8 --verbose $(DEFINES)
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
COPY          = cp -f
DEL_FILE      = rm -rf
DEL_DIR       = rmdir
LINK          = sdcc
LFLAGS        = --std-sdcc99 --opt-code-size -mstm8
LIBS          = $(SUBLIBS)  
SED           = sed
STRIP         = 

first: all
####### Implicit rules

.SUFFIXES: .rel .c .cpp .cc .cxx .C

.cpp.rel:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(BUILDDIR)/"$@" "$<"

.cc.rel:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(BUILDDIR)/"$@" "$<"

.cxx.rel:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(BUILDDIR)/"$@" "$<"

.C.rel:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(BUILDDIR)/"$@" "$<"

.c.rel:
	$(CC) -c $(CFLAGS) $(INCPATH) -o $(BUILDDIR)/"$@" "$<"

####### Build rules

all: $(TARGET)
debug: $(TARGET)
release: $(TARGET)

dirs:
	@test -d $(BUILDDIR)/ || mkdir -p $(BUILDDIR)/
	@test -d $(DESTDIR)/ || mkdir -p $(DESTDIR)/

$(TARGET): dirs $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET).hex $(OBJECTS_W_PATH) $(LIBS)
	objcopy -Iihex -Obinary $(DESTDIR)/$(TARGET).hex $(DESTDIR)/$(TARGET).bin

clean: 
	-$(DEL_FILE) $(BUILDDIR)/*
	-$(DEL_FILE) $(DESTDIR)/* *.lst *.rel *.rst *.sym *.asm *.map *.mem *.lk *.adb *.ihx *.bin

distclean: clean 

program: $(TARGET)
	stm8flash -p stm8l052c6 -c stlinkv2 -w $(DESTDIR)/$(TARGET).hex

 
