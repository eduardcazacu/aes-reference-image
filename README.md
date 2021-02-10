# aes-reference-image
Embedded Linux OS image for Zyboz Z7 designed for the Fontys EEE Advanced Embedded Systems course.

# Build instructions
* Use Petalinux 2019.1 
* Follow the instructions here https://www.xilinx.com/support/documentation/sw_manuals/xilinx2019_1/ug1144-petalinux-tools-reference-guide.pdf

# Packaging image
```bash
$petalinux-package --boot --fsbl images/linux/zynq_fsbl.elf --fpga images/linux/system.bit --u-boot --force
```
# Boot
* Compile and package image or pick one from the [releases tab](https://github.com/eduardcazacu/aes-reference-image/releases)
* Load image.ub and BOOT.BIN on the sd card
* power up the Zybo
