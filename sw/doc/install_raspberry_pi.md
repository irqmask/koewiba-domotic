# Installing operating system images on Linux

sudo dd bs=4M if=2018-11-13-raspbian-stretch-lite.img of=/dev/sdc

## Activate ssh

Create an empty file ssh in the boot partition of the SD-card. This will activate the ssh server on first boot of the raspberry pi. Now remove the SD card from the card reader.

## Boot the first time
Connect the Raspberry Pi to your network. Asure a DHCP service is running in your network. Most probably this is the standard internet router configuration.
Find the Raspberry Pi using nmap. From a computer in your network run:

```shell
ifconfig
# E.g. your computers get the 10.1.1.X IP addresses.
sudo apt install nmap
nmap 10.1.1.*
# Find a computer called Raspberry
ssh pi@<ip-address>
# type the standard password
```

## run system updates

```
sudo apt update
sudo apt dist-upgrade -y
```

# Configure Raspberry operating system

## Set hostname

* Set the hostname to a name you select: e.g. kwb1

  ```
  sudo nano /etc/hostname
  ```

* Power-cycle the Raspberry Pi
* Login again with ssh.

## Set IP address

Configure a static network adddress on your Pi in `/etc/network/interfaces`

`sudo nano /etc/network/interfaces`

Edit it to the following settings:

```
  auto eth0
  iface eth0 inet static
  address 10.1.1.30
  netmask 255.255.255.0
  gateway 10.1.1.1
```
Parameters:

* address is the address from the command above (or another unused address on the same network)
* netmask 255.255.255.0 corresponds to network size/24.
* gateway is the address of your router (or gateway).

You can also specify dns-nameservers, but this is generally not necessary.
The broadcast is automatically derived from address and netmask and need not be specified.
For more detail see https://wiki.debian.org/NetworkConfiguration

You can set either or both eth0 or wlan0

Then disable the DHCP client daemon and switch to standard Debian networking:

```
sudo systemctl disable dhcpcd
sudo systemctl enable networking
```

Reboot for the changes to take effect:

```
sudo reboot
```

## Change password
It is highly recommended to change the standard password of the Raspberry Pi!
Login to the Raspbarry Pi using the new address:
`ssh pi@<new_address>`
Using the standard password a almost last time.

Then type:
```
passwd
<your old password>
<your new password>
<repeat your new password>
```

Log-out and the log-in again using your new password.

## Configure

```
sudo raspi-config
->Set locale and timezone
->Interfacing Options->P6 Serial
    Disable serial login shell
    ENABLE the serial interface
->Interfacing Options->P5 I2C
    ENABLE I2C
->Advanced->A1 Expand Filesystem
exit with Finish. Let it reboot
```

## Install RTC

```
sudo apt install i2c-tools
sudo i2cdetect -y 1

     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- 68 -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --      
```

Ok, the 68 is the RTC chip.
Now the kernel module needs to be added:

`sudo nano /etc/modules`

Add `rtc-ds1307` at the end of the file.

```
sudo nano /etc/rc.local
(add before the "exit 0" line:)
echo ds1307 0x68 > /sys/class/i2c-adapter/i2c-1/new_device
sudo hwclock -s
```

Now the raspberry is ready for koewiba-domotic software.

## Install log2ram

To avoid heavy use of the SD card, log2ram is a perfect tool.
It log everything to RAM and writes the contents after a fixed interval or on reboot / shutdown to the SD card.

```
sudo apt install git
git clone https://github.com/azlux/log2ram.git
cd log2ram
sudo ./install.sh
sudo reboot
```

## Build linux components of koewiba

See [build_os_software.md]

# Install additional components

## 
## Install the built kwbrouter + kwbmqttgateway deamons as services


Copy the systemd init scripts (../sw/os/tools/)
    - kwbrouter.service.server_template
    - kwbmqttgateway.service
into `/etc/systemd/system`

Then the services need to be activated:

```
sudo systemctl daemon-reload
sudo systemctl enable kwbrouter
sudo systemctl enable kwbmqttgateway
```

## Install openHab

### Install Zulu (open source Java alternative especially for ARM)

Download Zulu install package from:   https://www.azul.com/downloads

Choose the java version that is recommended by openHAB web site (e.g. Java 11 for openHAB 3)
and download the 'tar.gz'-package for Linux OS and ARM 32-bit HF architecture.

Transfer package to Pi and unpack it into the home directory.


Extract the package and copy contents to `/opt/zulu`

```
tar -xzf zulu8.36.0.152-ca-jdk1.8.0_202-linux_aarch32hf.tar.gz
sudo mkdir -p /opt/zulu
sudo mv zulu8.36.0.152-ca-jdk1.8.0_202-linux_aarch32hf/* /opt/zulu/
```

Set java alternative

```
sudo update-alternatives --install /usr/bin/java java /opt/zulu/bin/java 100
java -version
```

You should see the installed Java version now.
 
### Install OpenHab
Follow the steps on the openhab homepage:
https://www.openhab.org/docs/installation/linux.html#package-repository-installation

