# aes-reference-image
Embedded Linux OS image for Zyboz Z7 designed for the Fontys EEE Advanced Embedded Systems course.


# Features
## Working
* Networking
* WiFi (with USB adapter)
* Persistent storage
* XADC
* Audio monitor firmare by [Tijntj3](https://github.com/Tijntj3/Zybo-Z7_Audiomonitor) (A bug with the fft library still needs to be fixed)
* Device Tree Overlay

## Untested
* PWM and Encoder PL modules
* Z7-10 images

## Not Implemented 
* ROS

## TODO
* Nicely packaged .img files for Windows users to flash on SD cards without needing to partition the SD card first


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

The master-z7-20 will have the ./hdf/default/zybo-z7-20/ configuration preselected.


# Packaging image
```bash
$ petalinux-package --boot --fsbl images/linux/zynq_fsbl.elf --fpga images/linux/system.bit --u-boot --force
```

The package BOOT.bin, image.ub and rootfs.tar.gz will be available in ./images/linux/

# FPGA Firmware

The pre-built images are available with two different FPGA firmwares (selectable through DTO)

## Default

Contains:

* Two PWM controllers
* Two Encoder Controllers
* GPIO Controller (Buttons, switches, LEDs)
* XADC

### Enabling
To use the hardware, the the device tree needs to be changed using Device Tree Overlay:

```bash
fpgautil -b /lib/firmware/top_level_wrapper_z7-20/top_level_wrapper.bit.bin -o /lib/firmware/top_level_wrapper_z7-20/system-user-default-z7-20.dtbo -f Full
```

Note: if using Z7-10 use the correct directories and file names.

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

# Device Tree Overlay (DTO)
Custom FPGA bitstreams and device trees can be used with this image. Only full bistream overwrite is supported.

A Xilinx guide is provided [here](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager).

The steps for using DTO are:
1. Generate bitstream in vivado
2. Export hardware (for Xilinx SDK) and also a .hdf file
3. Generate .bif file in Xilinx SDK
4. Get a byte-swapped .bin file from the .bif file using bootgen:
```bash
bootgen -image Full_Bitstream.bif -arch zynq -process_bitstream bin
```
3. Get the device tree file from the .hdf. For this, see steps in the [DTO/ README.md](DTO/README.md).
4. Edit the pl.dtsi device tree with your changes (the ones usually found in system-user.dtsi in petalinux projects).
5. Compile the pl.dtbo file as described in the [DTO/ README.md](DTO/README.md).
6. make sure the .bin file name matches the "firmware-name" in pl.dtsi.
6. Transfer the .bit.bin and .dtbo files to the Zybo.
7. Load the DTO using fpgautil (or manually with configfs if that is preffered):
```bash
fpgautil -b <your bitstream> -o <your .dtbo> -f Full
```
 if it fails due to "device tree already found in live tree" you can reset the device tree by:
 ```bash
rmdir /configfs/device-tree/overlays/full
```
then calling fpgautil again.

confirm successfull application by calling ```dmesg```.

# Audio Monitor functionality (DTO)
To enable the non-default Audio Monitor functionality of this image, Device Tree Overlay (DTO) is used. With DTO, the necessary fpga bitstream and device tree fragment is loaded. More information on this can be found on [this Xilinx confluence page](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager).

## Usage
use fpgautil to load the bitstream and DTO:
```bash
fpgautil -b /lib/firmware/miniproject_z7-20/miniproject_top.bit.bin -o /lib/firmware/miniproject_z7-20/system-user-audio-z7-20.dtbo -f Full
```

note: the default .dtbo files found in /lib/firmare/ directories are missing necessary modifications. Use the system-user-*.dtbo files instead

to reset the live tree to default:
```bash
cd /configfs/device-tree/overlays
rmdir full
```


or
```bash
fpgautil -R
```


call audiomonitor or audiomonitor-local

Note: currently audiomonitor (built from git source) does not work, use audiomonitor-local

### Reverting
To revert back to the default bitstream and DTO

```bash
fpgautil -b /lib/firmware/top_level_wrapper_Z7-20/top_level_wrapper.bit.bin -o /usr/firmware/system-user-default-z7-20.dtbo -f Full
```

## Development
For how to generate a .DTBO and .bit file read the [README.md in ./DTO/](DTO/README.md)

# ROS
Robot operating system - Work in progress

## Work done so far
* The meta-ros layer is available as a submodule of this repository
* Attempts to add the required layers failed during petalinux-config stage.

## Resources
[Xilinx forum discussion](https://forums.xilinx.com/t5/Embedded-Linux/Adding-ROS-to-Petalinux-which-recipes-to-include/td-p/1187716)


