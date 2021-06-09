This directory contains the Device Tree Overlay related files. The files and instructions here are mainly used to build the device tree overlay files for the Audio-Monitor project. 

## DTO kernel config:
follow the kernel config [here](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager).

## DTO Petalinux config:
petalinux-config -DTG Settings -> devicetree flags (-@) \
from  [here](https://forums.xilinx.com/t5/Embedded-Linux/Why-DTBO-fails-on-load-and-says-no-symbols-in-root-of-device/td-p/1088920).

## Generating the Device Tree overlay files:

1. Export the bit.bin file from vivado (Change project preferences to export .bin)
2. Copy the .hdf file and dt_overlay.tcl script into the same dir
3. edit dt_overlay.tcl with hdf path, processor type (ps7_cortexa9_0) and repo (device-tree-xlnx) path. Make sure the repo path is set to the petalinux project version (2019.1)
4. run `$ xsct dt_overlay.tcl overlay` . This will generate the pl.dtsi. Make any additions to the device tree now.
5. compile the dtbo by running `$ dtc -I dts -O dtb -o pl.dtbo -b 0 -@ dtg_out/pl.dtsi` . If compilation fails make sure dtc version 1.6.0 is used (`$ dtc -v`)


## Resources:
https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18841645/Solution+Zynq+PL+Programming+With+FPGA+Manager \
https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/18842279/Build+Device+Tree+Blob