# aes-reference-image
Embedded Linux OS image for Zyboz Z7 designed for the Fontys EEE Advanced Embedded Systems course.


# Features
## Working
* Networking
* WiFi (with USB adapter)

## Untested
* Device Tree Overlay
* PWM and Encoder PL modules

## Not Implemented 
* XADC
* Audio processor hardware and firmare by [Tijntj3](https://github.com/Tijntj3/Zybo-Z7_Audiomonitor)
* ROS
* Xilinx SDK toolchain
* Zybo Z7-10 version
* Persistent storage

# Usage
## Access
The device can be acessed via UART or SSH via Ethernet or WLAN (After configuration)
### Uart
1. Connect via micro USB cable
2. Use a serial terminal (Putty on Widnows or Screen on Linux). Baudrate is 115200

on Linux:
```bash
$ screen /dev/ttyUSB1 115200
```

### SSH
1. Connectvia ethernet cable or WiFi
2. From a device on the same network use an ssh client (Putty or WSL on Windows, ssh on linux)

```bash
$ ssh root@<Zybo IP>
```
The password is root. It will fail on first attempt.

## WiFi
The device supports RTL8192CU chipset-based usb WiFi adapters (Most low-cost Wireless addapter use this chipset). Other chipsets are not supported.

1. Connect a wireless adapter to the Zybo's USB port
2. Access the device via UART or ssh over Ethernet
3. Edit /etc/wpa_supplicant.conf with your network information
```bash
$ vi /etc/wpa_supplicant.conf
```
Once inside vim, press "i" to start editing the file

```bash
ctrl_interface=/var/run/wpa_supplicant
ctrl_interface_group=0
update_config=1

network={
        ssid="your_ssid"
        psk="your_wifi_password"
}
```

When done, press escape and then type ```:wq``` to save the changes and exit

4. Connect
```bash
$ wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf
```
5. Get an IP from the router
```bash
udhcpc -i wlan0
```

## Known issues 
* SSH authentication fails on first attempt. [info](https://forums.xilinx.com/t5/Embedded-Linux/First-Password-Attempt-Fails/td-p/985191)

# Build instructions
* Use Petalinux 2019.1 
* Follow the instructions [here](https://www.xilinx.com/support/documentation/sw_manuals/xilinx2019_1/ug1144-petalinux-tools-reference-guide.pdf)

# Packaging image
```bash
$petalinux-package --boot --fsbl images/linux/zynq_fsbl.elf --fpga images/linux/system.bit --u-boot --force
```
# Boot
* Compile and package image or pick one from the [releases tab](https://github.com/eduardcazacu/aes-reference-image/releases)
* Load image.ub and BOOT.BIN on the sd card's BOOT partition
* (If the release requires it) unzip the rootfs and load it on the rootfs SD card partition
```bash
$sudo tar xvf rootfs.tar.gz -C /media/rootfs
```
* power up the Zybo
* login: root, pwd: root
