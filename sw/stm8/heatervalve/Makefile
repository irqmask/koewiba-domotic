#############################################################################
# Makefile for building STM8 program
#############################################################################

MAKEFILE      = Makefile

####### Files

TARGET        = heatervalve

#../STM8L15x_StdPeriph_Driver/src/stm8l15x_aes.c
#../STM8L15x_StdPeriph_Driver/src/stm8l15x_beep.c
#                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_dac.c \
#                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_dma.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_i2c.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_irtim.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_itc.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_iwdg.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_wfe.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_wwdg.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_tim3.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_tim4.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_tim5.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_comp.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_flash.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_adc.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_rtc.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_exti.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_tim2.c \
                
SOURCES       = ../STM8L15x_StdPeriph_Driver/src/stm8l15x_clk.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_gpio.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_lcd.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_pwr.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_rst.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_spi.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_syscfg.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_tim1.c \
                ../STM8L15x_StdPeriph_Driver/src/stm8l15x_usart.c \
                adc.c \
                cmd.c \
                control_temp.c \
                lcd.c \
                main.c \
                motor.c \
                remote_tempsense.c \
                stm8l15x_ipt.c \
                timer.c \
                uart.c \

INCPATH       = -I. -I/usr/local/share/sdcc/include -I../STM8L15x_StdPeriph_Driver/inc

####### Output directories

BUILDDIR      = ../build/$(TARGET)
BUILDDIRSPL   = ../build/STM8L15x_StdPeriph_Driver/src
DESTDIR       = ../bin

OBJECTS       = $(SOURCES:.c=.rel)
OBJECTS_W_PATH= $(addprefix $(BUILDDIR)/,$(OBJECTS))

####### Compiler, tools and options

CC            = sdcc
CXX           = sdcc
DEFINES       = -DSTM8L05X_MD_VL -DHAS_APPCONFIG_H
CFLAGS        = -mstm8 --verbose -opt-code-size $(DEFINES)
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
	@test -d $(BUILDDIRSPL) || mkdir -p $(BUILDDIRSPL) 
	@test -d $(DESTDIR)/ || mkdir -p $(DESTDIR)/

$(TARGET): dirs $(OBJECTS)
	$(LINK) $(LFLAGS) -o $(DESTDIR)/$(TARGET).ihx $(OBJECTS_W_PATH) $(LIBS)
	objcopy -Iihex -Obinary $(DESTDIR)/$(TARGET).ihx $(DESTDIR)/$(TARGET).bin

clean:
	-$(DEL_FILE) $(BUILDDIR)/*
	-$(DEL_FILE) $(DESTDIR)/* *.lst *.rel *.rst *.sym *.asm *.map *.mem *.lk *.adb *.ihx *.bin

distclean: clean

program: $(TARGET)
	stm8flash -p stm8l052c6 -c stlinkv2 -w $(DESTDIR)/$(TARGET).ihx


