## Build software

tested on Ubuntu 18.04

### Common required packages

Install required packages

    sudo apt install build-essential git cmake libmosquitto-dev nlohmann-json3-dev
Install required packages to build AVR firmware:

    sudo apt install avr-libc avrdude gcc-avr
    
If you need a local MQTT server you can install 'mosquitto' and 'mosquitto-clients'

    sudo apt install mosquitto mosquitto-clients

Clone repository:

    git clone https://github.com/irqmask/koewiba-domotic.git
    cd koewiba-domotic
    
Create thridparty folder, build and install thirdparty libraries

    mkdir -p sw/os/3rd_party
    cd sw/os/3rd_party

Build libsafec:
    
    scp libsafec-10052013.tar.gz pi@192.168.X.Y:/home/<user>/koewiba-domotic/sw/os/3rd_party
    tar -xvvf libsafec-10052013.tar.gz
    mv libsafec-10052013 libsafec  
    cd libsafec
    ./configure
    make
    sudo make install
    sudo ldconfig           # recreate library index
    cd ../../../..

### Build AVR firmware

    cd sw/avr/buildscripts
    make clean
    make
    cd ../../..

Compiled firmware in Intel-HEX format is safed in 'sw/avr/bin/[Debug|Release]'
Detailled build logs are saved in the 'build/[applicationname]/[Debug/Release]
    
### Build Linux applications

    cd sw/os/
    mkdir build
    cd build
    cmake ..
    make -j4
    sudo make install

The built binaries are stored in 'sw/os/bin'. Build intermediates are stored in the 'sw/os/build' folder.
