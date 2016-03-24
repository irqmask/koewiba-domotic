TARGETS = basic_app_atmega324p basic_app_atmega328p bootloader_atmega324p bootloader_atmega328p roomthermostate scheduler_attiny1634 taster8  tests/elektor_bus-node_atmega88 tests/test_bus-module_atmega324p tests/test_bus-module_atmega328p tests/test_bus-module_attiny1634 tests/testgateway232_atmega324p

SUBDIRS = $(addprefix ../,$(TARGETS))
