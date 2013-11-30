TARGETS = bootloader_atmega324p roomthermostate taster8 tests/testscheduler

SUBDIRS = $(addprefix ../src/,$(TARGETS))
