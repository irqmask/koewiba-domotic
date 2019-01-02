TARGETS = bootloader_atmega324p roomthermostat taster8 tests/testscheduler

SUBDIRS = $(addprefix ../,$(TARGETS))
