# Program STM8 devices

## Prerequisites

### Linux Toolchain

Needed to compile and link the firmware and program the device. Tested on Debian like OS.

* Install 'Build essentials'

        sudo apt install build-essential git

* Small device C compiler (sdcc)

        sudo apt install sdcc

* Tool to program the device (stm8flash)

    Go to a folder outside of the koewiba repository suitable to checkout 3rd-Party apps to and clone the repository:

        git clone https://github.com/vdudouyt/stm8flash

    Then follow the instructions in the INSTALL file which is contained in the stm8flash repository.

        cd stm8flash
        cat INSTALL

### STM8/32 Windows Toolchain

Needed to reset the write protect fuse / option byte.

* en.stsw-link009_v2.0.2.zip (Driver)
* en.stvp-stm8_v42.0.0.zip (Tool)

## Reset programming fuse

In order to program the STM8 microcontroller, the write and read protect fuse (option byte) must be reset. E.g. this is the case with comercial EQ3 heater controllers.

On a windows PC or virtual machine:

1. Install both en.stsw-link009_v2.0.2.zip and en.stvp-stm8_v42.0.0.zip
2. Connect STlink v2 and verify device driver is loaded correctly
3. Open STVP.exe
4. Go to "OPTION BYTE" tab and set ROP to "Read Out Protection: OFF"
5. Click on "Program current tab or active sectors"
   ATTENTION: THIS WILL ERASE THE READOUT PROTECTED FLASH!!! YOU DEFINETLY NEED A CUSTOM FIRMWARE AFTER THIS STEP!!! ;-)
6. Now go back to Linux

## Build and program stm8 firmware

1. Change into project directory e.g.:

        cd sw/stm8/heatervalve

2. Edit code, compile  and program

        make
        make program
