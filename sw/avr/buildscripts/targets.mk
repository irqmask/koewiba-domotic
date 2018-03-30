TARGETS = basic_app_atmega324p basic_app_atmega328p basic_app_attiny1634 blind-control_atmega328p bootloader_atmega324p bootloader_atmega328p gateway232_atmega324p pushbutton8 roomthermostate scheduler_attiny1634 tests/elektor_bus-node_atmega88 tests/test_bus-module_atmega324p tests/test_bus-module_atmega328p tests/test_bus-module_attiny1634

SUBDIRS = $(addprefix ../,$(TARGETS))
