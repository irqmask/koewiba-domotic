# Build AVR architecture targets
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Build toolchain
CC               = avr-gcc
OBJCOPY          = avr-objcopy
OBJDUMP          = avr-objdump
SIZE             = avr-size
NM               = avr-nm
AVRDUDE          = avrdude
REMOVE           = rm
MV               = mv -f

# Make lists of source and object files
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

SRC_FULL         = $(addprefix $(SRCDIR)/,$(SRC))
SRC_PATHS        = $(sort $(dir $(SRC_FULL)))

SRC_C            = $(notdir $(filter %.c, $(SRC)))
SRC_S            = $(notdir $(filter %.S, $(SRC)))

OBJ_DEBUG        = $(addprefix $(BUILDDIR_DEBUG), $(SRC_C:.c=.o))
OBJ_DEBUG       += $(addprefix $(BUILDDIR_DEBUG), $(SRC_S:.S=.o))

OBJ_RELEASE      = $(addprefix $(BUILDDIR_RELEASE), $(SRC_C:.c=.o))
OBJ_RELEASE     += $(addprefix $(BUILDDIR_RELEASE), $(SRC_S:.S=.o))

vpath %.c $(SRC_PATHS)
vpath %.S $(SRC_PATHS)


# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile


# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99

CFLAGS_DEBUG = -g$(DEBUG)
CFLAGS_RELEASE =

CWARN = -Wall -Wstrict-prototypes
CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

CFLAGS = -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA) $(LOCAL_CFLAGS)
ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp
#ASFLAGS += -Wa,-adhlns=$(<:.S=.lst),-gstabs


#Additional libraries.

MATH_LIB = -lm

# External memory options

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# used for variables (.data/.bss) and heap (malloc()).
#EXTMEMOPTS = -Wl,--section-start,.data=0x801100,--defsym=__heap_end=0x80ffff

# 64 KB of external RAM, starting after internal RAM (ATmega128!),
# only used for heap (malloc()).
#EXTMEMOPTS = -Wl,--defsym=__heap_start=0x801100,--defsym=__heap_end=0x80ffff

EXTMEMOPTS =

LDMAP = -Wl,-Map=$(MAPFILE),--cref

LDFLAGS = $(EXTMEMOPTS) $(LDMAP) $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB) $(LOCAL_LDFLAGS)

# Define all object files.
OBJ = $(SRC:.c=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(SRC:.c=.lst)


# Debug build target (Default)
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

build_debug: IDENT = "Debug: $(TARGET)"
build_debug: CFLAGS += $(CFLAGS_DEBUG)
build_debug: BINDIR = $(BINDIR_DEBUG)
build_debug: BUILDDIR = $(BUILDDIR_DEBUG)
build_debug: LOGFILE = $(LOGFILE_DEBUG)
build_debug: OBJFILES = $(OBJ_DEBUG)
build_debug: ELFFILE = $(ELFFILE_DEBUG)
build_debug: MAPFILE = $(MAPFILE_DEBUG)
build_debug: HEXFILE = $(HEXFILE_DEBUG)
build_debug: LSSFILE = $(LSSFILE_DEBUG)
build_debug: SYMFILE = $(SYMFILE_DEBUG)
build_debug: directories $(OBJ_DEBUG) $(ELFFILE_DEBUG) $(HEXFILE_DEBUG) $(EEPFILE_DEBUG) debug_done

# Release build target
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

build_release: IDENT = "Release: $(TARGET)"
build_release: CFLAGS += $(CFLAGS_RELEASE)
build_release: BINDIR = $(BINDIR_RELEASE)
build_release: BUILDDIR = $(BUILDDIR_RELEASE)
build_release: LOGFILE = $(LOGFILE_RELEASE)
build_release: OBJFILES = $(OBJ_RELEASE)
build_release: ELFFILE = $(ELFFILE_RELEASE)
build_release: MAPFILE = $(MAPFILE_RELEASE)
build_release: HEXFILE = $(HEXFILE_RELEASE)
build_release: LSSFILE = $(LSSFILE_RELEASE)
build_release: SYMFILE = $(SYMFILE_RELEASE)
build_release: directories $(OBJ_RELEASE) $(ELFFILE_RELEASE) $(HEXFILE_RELEASE) $(EEPFILE_RELEASE) release_done

debug_done: $(ELFFILE_DEBUG)
	@echo "    $(IDENT) [DONE]"

release_done: $(ELFFILE_RELEASE)
	@echo "    $(IDENT) [DONE]"

all: build_debug

# Compile: create object files from C source files.
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

$(BUILDDIR_DEBUG)%.o $(BUILDDIR_RELEASE)%.o: %.c
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [CC]         ===> $<"
endif
	@echo "$(CC) -mmcu=$(MCU) -c $(CFLAGS) $(INCDIRS) -o $@ $<" >> $(LOGFILE) 2>&1
	@$(CC) -mmcu=$(MCU) -c $(CFLAGS) $(INCDIRS) -o $@ $< >> $(LOGFILE) 2>&1

# Assemble: create object files from assembler source files.
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

$(BUILDDIR_DEBUG)%.o $(BUILDDIR_RELEASE)%.o: %.S
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [CC]         ===> $<"
endif
	@echo "$(CC) -mmcu=$(MCU) -c $(ASFLAGS) $(INCDIRS) -o $@ $<" >> $(LOGFILE) 2>&1
	@$(CC) -mmcu=$(MCU) -c $(ASFLAGS) $(INCDIRS) -o $@ $< >> $(LOGFILE) 2>&1


# Link executable
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

$(ELFFILE_DEBUG) $(ELFFILE_RELEASE): $(SRC_FULL) $(LIBRARIES)
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [LD]         ===> $@"
endif
	@echo "$(CC) -o $@ $(OBJFILES) $(LDFLAGS) " >> $(LOGFILE) 2>&1
	@$(CC) -mmcu=$(MCU) -o $@ $(OBJFILES) $(LDFLAGS) >> $(LOGFILE) 2>&1
	@$(SIZE) --format=avr --mcu=$(MCU) $@ >> $(LOGFILE) 2>&1


# several object-copy targets
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

.elf.hex:
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [OBJCOPYHEX] ===> $<"
endif
	@echo "$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@" >> $(LOGFILE) 2>&1
	@$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@ >> $(LOGFILE) 2>&1

.elf.eep:
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [OBJCOPYEEP] ===> $<"
endif
	@echo "-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@" >> $(LOGFILE) 2>&1
	@-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@ >> $(LOGFILE) 2>&1


# Create extended listing file from ELF output file.
.elf.lss:
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [OBJDUMP]    ===> $<"
endif
	@echo "$(OBJDUMP) -h -S $< > $@" >> $(LOGFILE) 2>&1
	@$(OBJDUMP) -h -S $< > $@ >> $(LOGFILE) 2>&1


# Create a symbol table from ELF output file.
.elf.sym:
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [NM]         ===> $<"
endif
	@echo "$(NM) -n $< > $@" >> $(LOGFILE) 2>&1
	@$(NM) -n $< > $@ >> $(LOGFILE) 2>&1


# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
--change-section-address .data-0x800000 \
--change-section-address .bss-0x800000 \
--change-section-address .noinit-0x800000 \
--change-section-address .eeprom-0x810000


coff: $(TARGET).elf
	$(COFFCONVERT) -O coff-avr $(BINDIR)/$(TARGET).elf $(BINDIR)/$(TARGET).cof


extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr $(TARGET).elf $(TARGET).coff

# create directories for build
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

directories:
	@mkdir -p $(BINDIR)
	@mkdir -p $(BUILDDIR)


# Target: clean project.
# =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

clean: clean_debug clean_release

clean_debug:
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [RM]     ===> $(ELFFILE_DEBUG)"
	@echo "    $(IDENT) [RM]     ===> $(HEXFILE_DEBUG)"
	@echo "    $(IDENT) [RM]     ===> $(LSSFILE_DEBUG)"
	@echo "    $(IDENT) [RM]     ===> $(SYMFILE_DEBUG)"
	@echo "    $(IDENT) [RM]     ===> $(EEPFILE_DEBUG)"
	@echo "    $(IDENT) [RM]     ===> $(MAPFILE_DEBUG)"
	@echo "    $(IDENT) [RM]     ===> $(LOGFILE_DEBUG)"
endif
	@$(REMOVE) -f $(ELFFILE_DEBUG) $(HEXFILE_DEBUG) $(LSSFILE_DEBUG) \
                  $(SYMFILE_DEBUG) $(EEPFILE_DEBUG) $(MAPFILE_DEBUG) $(LOGFILE_DEBUG)
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [RM]     ===> $(BUILDDIR_DEBUG)"
endif
	@$(REMOVE) -rf $(BUILDDIR_DEBUG)

clean_release:
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [RM]     ===> $(ELFFILE_RELEASE)"
	@echo "    $(IDENT) [RM]     ===> $(HEXFILE_RELEASE)"
	@echo "    $(IDENT) [RM]     ===> $(LSSFILE_RELEASE)"
	@echo "    $(IDENT) [RM]     ===> $(SYMFILE_RELEASE)"
	@echo "    $(IDENT) [RM]     ===> $(EEPFILE_RELEASE)"
	@echo "    $(IDENT) [RM]     ===> $(MAPFILE_RELEASE)"
	@echo "    $(IDENT) [RM]     ===> $(LOGFILE_RELEASE)"
endif
	@$(REMOVE) -f $(ELFFILE_RELEASE) $(HEXFILE_RELEASE) $(LSSFILE_RELEASE) \
                  $(SYMFILE_RELEASE) $(EEPFILE_RELEASE) $(MAPFILE_RELEASE) \
                  $(LOGFILE_DEBUG)
ifneq ($(BUILD_NIGHTLY),True)
	@echo "    $(IDENT) [RM]     ===> $(BUILDDIR_RELEASE)"
endif
	@$(REMOVE) -rf $(BUILDDIR_RELEASE)


.SUFFIXES: .elf .hex .eep .lss .sym

depend:
	if grep '^# DO NOT DELETE' $(MAKEFILE) >/dev/null; \
	then \
		sed -e '/^# DO NOT DELETE/,$$d' $(MAKEFILE) > \
			$(MAKEFILE).$$$$ && \
		$(MV) $(MAKEFILE).$$$$ $(MAKEFILE); \
	fi
	echo '# DO NOT DELETE THIS LINE -- make depend depends on it.' \
		>> $(MAKEFILE); \
	$(CC) -M -mmcu=$(MCU) $(CDEFS) $(CINCS) $(SRC) $(ASRC) >> $(MAKEFILE)

.PHONY:	all build_debug build_release debug_done release_done elf hex eep lss sym  directories clean_debug clean_release program coff extcoff depend
