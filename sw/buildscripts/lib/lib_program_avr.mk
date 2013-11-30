# Programming support using avrdude. Settings and variables.

AVRDUDE_PROGRAMMER            = avrisp2
AVRDUDE_PORT                  = usb

AVRDUDE_WRITE_FLASH_DEBUG     = -U flash:w:$(HEXFILE_DEBUG)
AVRDUDE_WRITE_FLASH_RELEASE   = -U flash:w:$(HEXFILE_RELEASE)

#AVRDUDE_WRITE_EEPROM_DEBUG   = -U eeprom:w:$(EEPFILE_DEBUG)
#AVRDUDE_WRITE_EEPROM_RELEASE = -U eeprom:w:$(EEPFILE_RELEASE)

# Uncomment the following if you want avrdude's erase cycle counter.
# Note that this counter needs to be initialized first using -Yn,
# see avrdude manual.
#AVRDUDE_ERASE_COUNTER = -y

# Uncomment the following if you do /not/ wish a verification to be
# performed after programming the device.
#AVRDUDE_NO_VERIFY = -V

# Increase verbosity level.  Please use this when submitting bug
# reports about avrdude. See <http://savannah.nongnu.org/projects/avrdude>
# to submit bug reports.
#AVRDUDE_VERBOSE = -v -v

AVRDUDE_BASIC = -p $(MCU) -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS = $(AVRDUDE_BASIC) $(AVRDUDE_NO_VERIFY) $(AVRDUDE_VERBOSE) $(AVRDUDE_ERASE_COUNTER) -B 32

# Program the device.
program: program_debug

program_debug: build_debug $(HEXFILE_DEBUG) $(EEPFILE_DEBUG)
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH_DEBUG) $(AVRDUDE_WRITE_EEPROM_DEBUG)

program_release: build_release $(HEXFILE_RELEASE) $(EEPFILE_RELEASE)
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH_RELEASE) $(AVRDUDE_WRITE_EEPROM_RELEASE)
