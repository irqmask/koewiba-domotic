TARGETS = basic_app_atmega324p \
          basic_app_atmega328p \
          basic_app_attiny1634 \
          blind-control_atmega328p \
          blind-control_5_atmega328p \
          blind-control_5_atmega328pb \
          bootloader_atmega324p \
          bootloader_atmega328p \
          bootloader_atmega328pb \
          gateway232_atmega324p \
          led2_atmega324p \
          roomthermostat \
          scheduler_attiny1634 \
          switch5_atmega328p \
          tests/elektor_bus-node_atmega88 \
          tests/test_bus-module_atmega324p \
          tests/test_bus-module_atmega328p \
          tests/test_bus-module_attiny1634 \
          tests/test_motor_switch_5_atmega328p \
          tests/test_switch_5_atmega328p \
          tests/test_roomthermostat_atmega328

SUBDIRS = $(addprefix ../,$(TARGETS))
