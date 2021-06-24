# aes-reference-image
Embedded Linux OS image for Zyboz Z7 designed for the Fontys EEE Advanced Embedded Systems course.


# Features
## Working
* Networking
* WiFi (with USB adapter)
* Persistent storage
* XADC
* Audio monitor firmare by [Tijntj3](https://github.com/Tijntj3/Zybo-Z7_Audiomonitor) (A bug with the fft library still need to be fixed)
* Device Tree Overlay

## Untested
* PWM and Encoder PL modules
* Z7-10 images

## Not Implemented 
* ROS


# Usage
To use the image, it must be loaded onto a correctly formatted SD card and booted on the Zybo.

## Preparing the SD card
If SD card rootfs is desired (for persistent storage), the SD card should pe partitioned and formatted as follows:
1. 64MB FAT32 partition with 4MB of freee space proceeding. Label: BOOT
2. ext4 partition spanning the rest of the SD card. Label :rootfs

## Boot
* Build an image from source or pick one from the [releases tab](https://github.com/eduardcazacu/aes-reference-image/releases). Make sure to choose the correct board (Z7-10 or Z7-20)
* Load image.ub and BOOT.BIN on the sd card's BOOT partition
* (If the release requires it) unzip the rootfs and load it on the rootfs SD card partition
```bash
$ sudo tar xvf rootfs.tar.gz -C /media/<username>/rootfs
```
* power up the Zybo
* login: root, pwd: root

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

# Bitstream and device tree
Before building, when configuring the project, make sure to select the desired hardware description (.hdf):
```bash
$ petalinux-config --get-hw-description=<path-to-hdf>
```

for example, for the default z7-20 hdf, use the following path (from project root): ./hdf/default/zybo-z7-20/


# Packaging image
```bash
$ petalinux-package --boot --fsbl images/linux/zynq_fsbl.elf --fpga images/linux/system.bit --u-boot --force
```

The package BOOT.bin, image.ub and rootfs.tar.gz will be available in ./images/linux/

# FPGA Firmware

The pre-built images are available with two different FPGA firmwares:

## Default

Contains:

* Two PWM controllers
* Two Encoder Controllers
* GPIO Controller (Buttons, switches, LEDs)
* XADC

### PWM and Encoder Controllers
Useful for robotics applications. 

The memory/uio mapping is:

| Block     | UIO device | Start Address |
|-----------|------------|---------------|
| PWM 0     | /dev/uio0  | 0x43C0_0000   |
| PWM 1     | /dev/uio1  | 0x43C1_0000   |
| Encoder 0 | /dev/uio2  | 0x43C2_0000   |
| Encoder 1 | /dev/uio3  | 0x43C3_0000   |

The GPIO mapping is:

Pmod JC:

|  Pin 6 |     |         |         |         | Pin 0 |
|--------|-----|---------|---------|---------|-------|
| VCC    | GND | EN_0    | DIR_0   |         | PWM_0 |
| VCC    | GND | ENC_A_0 | ENC_B_0 | ENC_I_0 |       |

Pmod JD:

|  Pin 6 |     |         |         |         | Pin 1 |
|--------|-----|---------|---------|---------|-------|
| VCC    | GND | EN_1    | DIR_1   |         | PWM_1 |
| VCC    | GND | ENC_A_1 | ENC_B_1 | ENC_I_1 |       |


### GPIO Controller

LEDs address:   0x4120 0000
Btn address:    0x4120 0008 (bits 4 to 7)
Swt address:    0x4120 0008 (bits 0 to 3)

Example:
Turn LED1 on:
```bash
poke 0x41200000 0x2
```

Read switches /buttons
Turn LED1 on:
```bash
peek 0x41200008
```

Note: peek and poke programs are available from the command line in the image. In custom programs use mmap (memory mapped io).

### XADC
The XADC is mapped to PMod JA.

## Audio Monitor
The audio montor FPGA image contains the necessary hardware for using the Zybo audio CODEC.




# Audio Monitor functionality (Device Tree Overaly)
To enable the non-default Audio Monitor functionality of this image, Device Tree Overlay (DTO) is used. With DTO, the necessary fpga bitstream and device tree fragment is loaded. More information on this can be found on [this Xilinx confluence page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager).

## Usage
use fpgautil to load the bitstream and DTO:
```bash
fpgautil -b /lib/firmware/miniproject_z7-20/miniproject_top.bit.bin -o /lib/firmware/miniproject_z7-20/system-user.dtbo -f Full
```

to reset the live tree to default:
```bash
cd /configfs/device-tree/overlays
rmdir full
```


## Development
For how to generate a .DTBO and .bit file read the [README.md in ./DTO/](DTO/README.md)
