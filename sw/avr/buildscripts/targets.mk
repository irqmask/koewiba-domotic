TARGETS = basic_app_atmega324p basic_app_atmega328p basic_app_attiny1634 blind-control_atmega328p bootloader_atmega324p bootloader_atmega328p gateway232_atmega324p roomthermostat scheduler_attiny1634 taster8 tests/elektor_bus-node_atmega88 tests/test_bus-module_atmega324p tests/test_bus-module_atmega328p tests/test_bus-module_attiny1634 tests/test_roomthermostat_atmega328

SUBDIRS = $(addprefix ../,$(TARGETS))
